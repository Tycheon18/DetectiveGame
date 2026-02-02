// Fill out your copyright notice in the Description page of Project Settings.

#include "DetectiveNPC.h"
#include "../Components/DialogueComponent.h"
#include "../Components/InteractionComponent.h"
#include "../Components/CinematicCameraComponent.h"

// Sets default values
ADetectiveNPC::ADetectiveNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// DialogueComponent 생성
	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));

	// InteractionComponent 생성
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	// CinematicCameraComponent 생성
	CinematicCameraComponent = CreateDefaultSubobject<UCinematicCameraComponent>(TEXT("CinematicCameraComponent"));

	if (CinematicCameraComponent)
	{
		CinematicCameraComponent->bLookAtPlayer = true;
		CinematicCameraComponent->bRotateOwnerToPlayer = true;
		CinematicCameraComponent->BlendTime = 1.0f;
	}
}

// Called when the game starts or when spawned
void ADetectiveNPC::BeginPlay()
{
	Super::BeginPlay();

	// DialogueComponent 이벤트 바인딩
	if (DialogueComponent)
	{
		// Static Delegate가 아닌 멤버 Delegate 사용 (OnDialogueUpdated)
		// StartDialogue는 외부에서 호출되므로, 직접 바인딩보다는
		// InteractionComponent에서 StartDialogue 호출 시 카메라 활성화
		
		// 대화 종료 시 카메라 비활성화
		DialogueComponent->OnDialogueEnded.AddDynamic(this, &ADetectiveNPC::OnDialogueFinished);
	}

	// InteractionComponent 설정
	if (InteractionComponent)
	{
		// 인터랙션 시 대화 시작
		InteractionComponent->OnInteracted.AddDynamic(this, &ADetectiveNPC::OnDialogueStarted);
	}
}

// Called every frame
void ADetectiveNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADetectiveNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADetectiveNPC::OnDialogueStarted(AActor* InActor)
{
	// 카메라 활성화
	//if (CinematicCameraComponent)
	//{
	//	CinematicCameraComponent->ActivateCamera();
	//}

	UE_LOG(LogTemp, Log, TEXT("NPC Dialogue Started: %s"), *GetName());
}

void ADetectiveNPC::OnDialogueFinished()
{
	// 카메라 비활성화
	//if (CinematicCameraComponent)
	//{
	//	CinematicCameraComponent->DeactivateCamera();
	//}

	UE_LOG(LogTemp, Log, TEXT("NPC Dialogue Finished: %s"), *GetName());
}
