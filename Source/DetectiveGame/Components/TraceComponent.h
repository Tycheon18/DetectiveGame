// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DETECTIVEGAME_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UFUNCTION(BlueprintCallable)
	void TraceForInteractable();

	APlayerCameraManager* PlayerCameraManager = nullptr;
	class ADetectivePlayerController* PC = nullptr;

	FVector CameraLocation;
	FRotator CameraRotation;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class ADetectiveInteractable* InteractableActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ADetectiveInteractable*> NearActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInteractionComponent* InteractionComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UInteractionComponent*> NearComponents;

public:

	UFUNCTION(BlueprintCallable)
	bool GetIsNearestComponent(UInteractionComponent* NewComponent) const { return NearComponents.Contains(NewComponent); }

	UFUNCTION(BlueprintCallable)
	void AddNearComponent(UInteractionComponent* NewComponent);

	UFUNCTION(BlueprintCallable)
	void RemoveNearComponent(UInteractionComponent* OldComponent);
};
