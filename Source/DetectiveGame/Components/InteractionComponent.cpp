// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Components/SphereComponent.h"
#include "DialogueComponent.h"
#include "../DetectivePlayerState.h"
#include "../DetectiveManager.h"
#include "../DetectivePlayerController.h"
#include "../DetectiveCharacter.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InteractionSphere = nullptr;
	OverlapedCharacter = nullptr;
	ParentActor = nullptr;
	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	USphereComponent* SphereComp = GetOwner()->FindComponentByClass<USphereComponent>();

	if (SphereComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InteractionComponent] Bind Overlap Events"));

		InteractionSphere = SphereComp;

		InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::HandleBeginOverlap);
		InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::HandleEndOverlap);
	}
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InteractionSphere)
	{
		OverlapedCharacter = Cast<ADetectiveCharacter>(OtherActor);

		OverlapedCharacter->AddInteractComponent(this);
	}
}

void UInteractionComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InteractionSphere)
	{
		OverlapedCharacter = Cast<ADetectiveCharacter>(OtherActor);

		OverlapedCharacter->RemoveInteractComponent(this);
	}
}

bool UInteractionComponent::TryAcquireClue(ADetectivePlayerState* PS) const
{
	if(!PS)	return false;

	if(PS->IsClueActive(DetectiveNodeID))
	{
		return false;
	}

	PS->ActivateClue(DetectiveNodeID);

	return bClueActivated;
}

void UInteractionComponent::Interact(ADetectivePlayerController* InController)
{
	UE_LOG(LogTemp, Warning, TEXT("[InteractionComponent] Interact is Called"));


	if (InController)
	{
		if (bHasLinkedClue)
		{
			return;
		}
	
		ADetectivePlayerState* PS = Cast<ADetectivePlayerState>(InController->PlayerState);

		AActor* Owner = GetOwner();

		if (Owner)
		{
			UDialogueComponent* DialogueComp = Owner->FindComponentByClass<UDialogueComponent>();
			if(DialogueComp)
			{
				DialogueComp->StartDialogue(PS);
				UE_LOG(LogTemp, Log, TEXT("[InteractionComponent] Started dialogue via DialogueComponent"));

				return;
			}
		}

		TryAcquireClue(PS);

	}
}



