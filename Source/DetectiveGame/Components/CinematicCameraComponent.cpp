// Fill out your copyright notice in the Description page of Project Settings.

#include "CinematicCameraComponent.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UCinematicCameraComponent::UCinematicCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false; // 회전할 때만 활성화

	CameraPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPoint"));
}

void UCinematicCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CameraPoint && GetOwner())
	{
		CameraPoint->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		CameraPoint->SetRelativeLocation(FVector(200.f, 100.f, 80.0f));
	}

	CachedPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	CreateTempCamera();

}

void UCinematicCameraComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(bIsCameraActive)
	{
		DeactivateCamera();
	}

	if(TempCamera)
	{
		TempCamera->Destroy();
		TempCamera = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void UCinematicCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRotating && GetOwner())
	{
		FRotator CurrentRotation = GetOwner()->GetActorRotation();
		FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
		GetOwner()->SetActorRotation(NewRotation);

		if (CurrentRotation.Equals(TargetRotation, 1.0f))
		{
			bIsRotating = false;
			SetComponentTickEnabled(false);
			UE_LOG(LogTemp, Log, TEXT("[CinematicCamera] Owner rotation completed"));
		}
	}

	if (bShowDebugCamera)
	{
		DrawDebugVisualization();
	}
}

void UCinematicCameraComponent::ActivateCamera()
{
	if (!TempCamera || !CachedPlayerController || bIsCameraActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CinematicCamera] Cannot activate: TempCamera=%s, Controller=%s, Active=%d"),
			TempCamera ? TEXT("OK") : TEXT("NULL"),
			CachedPlayerController ? TEXT("OK") : TEXT("NULL"),
			bIsCameraActive);
		return;
	}

	PreviousViewTarget = CachedPlayerController->GetViewTarget();

	if (bLookAtPlayer)
	{
		RotateCameraPointToPlayer();
	}

	UpdateCameraTransform();

	CachedPlayerController->SetViewTargetWithBlend(TempCamera, BlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut);

	bIsCameraActive = true;

	if (bRotateOwnerToPlayer)
	{
		StartRotateToPlayer();
	}

	UE_LOG(LogTemp, Log, TEXT("[CinematicCamera] Camera activated on %s (BlendTime: %.2f)"),
		*GetOwner()->GetName(), BlendTime);
}

void UCinematicCameraComponent::DeactivateCamera()
{
	if (!CachedPlayerController || !bIsCameraActive)
	{
		return;
	}

	if (PreviousViewTarget)
	{
		CachedPlayerController->SetViewTargetWithBlend(PreviousViewTarget, BlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut);
	}

	bIsCameraActive = false;
	bIsRotating = false;
	SetComponentTickEnabled(false);

	UE_LOG(LogTemp, Log, TEXT("[CinematicCamera] Camera deactivated on %s"), *GetOwner()->GetName());
}

void UCinematicCameraComponent::CreateTempCamera()
{
	if (!GetWorld() || !GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("[CinematicCamera] Cannot create TempCamera: No World or Owner"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TempCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnParams);

	if (TempCamera)
	{
		TempCamera->SetActorLabel(FString::Printf(TEXT("TempCam_%s"), *GetOwner()->GetName()));
		UE_LOG(LogTemp, Log, TEXT("[CinematicCamera] TempCamera created: %s"), *TempCamera->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[CinematicCamera] Failed to create TempCamera"));
	}
}

void UCinematicCameraComponent::UpdateCameraTransform()
{
	if (!TempCamera || !CameraPoint)
	{
		return;
	}

	FTransform CameraTransform = CameraPoint->GetComponentTransform();
	TempCamera->SetActorTransform(CameraTransform);

	UE_LOG(LogTemp, Log, TEXT("[CinematicCamera] TempCamera updated to: %s"), *CameraTransform.GetLocation().ToString());
}

void UCinematicCameraComponent::StartRotateToPlayer()
{
	if (!GetOwner() || !CachedPlayerController)
	{
		return;
	}

	APawn* PlayerPawn = CachedPlayerController->GetPawn();
	if (!PlayerPawn)
	{
		return;
	}

	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - GetOwner()->GetActorLocation();
	DirectionToPlayer.Z = 0.0f;
	TargetRotation = DirectionToPlayer.Rotation();

	bIsRotating = true;
	SetComponentTickEnabled(true);

	UE_LOG(LogTemp, Log, TEXT("[CinematicCamera] Owner rotation started towards player"));
}

void UCinematicCameraComponent::RotateCameraPointToPlayer()
{
	if (!CameraPoint || !CachedPlayerController)
	{
		return;
	}

	APawn* PlayerPawn = CachedPlayerController->GetPawn();
	if (!PlayerPawn)
	{
		return;
	}

	FVector CameraLocation = CameraPoint->GetComponentLocation();
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FRotator LookAtRotation = (PlayerLocation - CameraLocation).Rotation();

	CameraPoint->SetWorldRotation(LookAtRotation);

	UE_LOG(LogTemp, Log, TEXT("[CinematicCamera] CameraPoint rotated to look at player"));

}

void UCinematicCameraComponent::DrawDebugVisualization()
{
	if (!GetWorld() || !CameraPoint || !TempCamera)
	{
		return;
	}

	FVector CameraLocation = CameraPoint->GetComponentLocation();
	FVector CameraForward = CameraPoint->GetForwardVector() * 100.0f;

	DrawDebugSphere(GetWorld(), CameraLocation, 20.0f, 8, FColor::Cyan, false, -1.0f, 0, 2.0f);

	DrawDebugDirectionalArrow(GetWorld(), CameraLocation, CameraLocation + CameraForward, 50.0f, FColor::Green, false, -1.0f, 0, 2.0f);

	DrawDebugString(GetWorld(), CameraLocation + FVector(0, 0, 30),
		bLookAtPlayer ? TEXT("NPC Camera") : TEXT("Clue Camera"),
		nullptr, FColor::White, 0.0f, true);
}
