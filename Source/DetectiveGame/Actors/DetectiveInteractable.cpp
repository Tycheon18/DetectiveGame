// Fill out your copyright notice in the Description page of Project Settings.

#include "DetectiveInteractable.h"
#include "Components/SphereComponent.h"
#include "../Components/InteractionComponent.h"
#include "../Components/CinematicCameraComponent.h"
#include "TimerManager.h"

// Sets default values
ADetectiveInteractable::ADetectiveInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	RootComponent = StaticMesh;

	InteractionSphere->SetupAttachment(RootComponent);
	InteractionComponent->SetSphereComponent(InteractionSphere);

	// CinematicCameraComponent 생성
	CinematicCameraComponent = CreateDefaultSubobject<UCinematicCameraComponent>(TEXT("CinematicCameraComponent"));

}

// Called when the game starts or when spawned
void ADetectiveInteractable::BeginPlay()
{
	Super::BeginPlay();

	// InteractionComponent 이벤트 바인딩
	if (InteractionComponent)
	{
		InteractionComponent->OnInteracted.AddDynamic(this, &ADetectiveInteractable::OnInteracted);
	}
}

// Called every frame
void ADetectiveInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADetectiveInteractable::OnInteracted(AActor* InActor)
{
	// 카메라 클로즈업 사용하지 않으면 리턴
	if (!bUseCloseUpCamera || !CinematicCameraComponent)
	{
		return;
	}

	// 카메라 활성화
	//CinematicCameraComponent->ActivateCamera();

	// 자동 복귀 타이머 설정 (CloseUpDuration > 0인 경우)
}

void ADetectiveInteractable::DeactivateCloseUpCamera()
{
	//if (CinematicCameraComponent)
	//{
	//	CinematicCameraComponent->DeactivateCamera();
	//}

	// 타이머 클리어
	if (CloseUpTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CloseUpTimerHandle);
	}

	UE_LOG(LogTemp, Log, TEXT("Interactable CloseUp Deactivated: %s"), *GetName());
}
