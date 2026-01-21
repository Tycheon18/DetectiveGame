// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectiveInteractable.h"
#include "Components/SphereComponent.h"
#include "../Components/InteractionComponent.h"


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
}

// Called when the game starts or when spawned
void ADetectiveInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADetectiveInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

