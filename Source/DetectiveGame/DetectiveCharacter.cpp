// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectiveCharacter.h"
#include "Components/TraceComponent.h"
#include "Components/InteractionComponent.h"

// Sets default values
ADetectiveCharacter::ADetectiveCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TraceComponent = CreateDefaultSubobject<UTraceComponent>("TraceComponent");
}

// Called when the game starts or when spawned
void ADetectiveCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADetectiveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADetectiveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADetectiveCharacter::AddInteractComponent(UInteractionComponent* NewComponent)
{
	if (!TraceComponent->GetIsNearestComponent(NewComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("[DetectiveCharacter] Add Interact Component %s"), *NewComponent->GetName());
		TraceComponent->AddNearComponent(NewComponent);
	}
}

void ADetectiveCharacter::RemoveInteractComponent(UInteractionComponent* OldComponent)
{
	if(TraceComponent)
	{
		TraceComponent->RemoveNearComponent(OldComponent);
	}
}

