// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "InteractionComponent.h"
#include "../DetectivePlayerController.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetComponentTickInterval(0.0625f);
	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	PC = Cast<ADetectivePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	TraceForInteractable();
}

void UTraceComponent::TraceForInteractable()
{
	CameraLocation = PlayerCameraManager->GetCameraLocation();
	CameraRotation = PlayerCameraManager->GetCameraRotation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(CameraRotation);
	FVector EndLocation = CameraLocation + (ForwardVector * 1000);
	FHitResult HitOut;
	FCollisionQueryParams TraceParams;

	GetWorld()->LineTraceSingleByChannel(HitOut, CameraLocation, EndLocation, ECC_Visibility, TraceParams);

	if (HitOut.GetActor())
	{
		AActor* HitActor = Cast<AActor>(HitOut.GetActor());
		UInteractionComponent* HitComponent = HitActor->GetComponentByClass<UInteractionComponent>();

		if(PC->GetInteractionComponent() == nullptr)
		{
			
		}

		if (HitComponent)
		{

			if (NearComponents.Contains(HitComponent))
			{
				InteractionComponent = HitComponent;
				//UE_LOG(LogTemp, Warning, TEXT("[Component] Hit Component: %s"), *HitComponent->GetName());
			}

			PC->SetInteractionComponent(this->InteractionComponent);

		}

	}
}

void UTraceComponent::AddNearComponent(UInteractionComponent* NewComponent)
{
	NearComponents.AddUnique(NewComponent);
}

void UTraceComponent::RemoveNearComponent(UInteractionComponent* OldComponent)
{
	if (NearComponents.Contains(OldComponent))
	{
		NearComponents.Remove(OldComponent);
	}
}
