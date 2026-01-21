// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../DetectiveStruct.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, Interactor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DETECTIVEGAME_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void HandleBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class USphereComponent* InteractionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class ADetectiveCharacter* OverlapedCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class ADetectiveInteractable* ParentActor;

protected:


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	int32 DetectiveNodeID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	bool bClueActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	bool bHasLinkedClue = false;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteracted OnInteracted;

public:

	UFUNCTION(BlueprintCallable)
	void SetSphereComponent(class USphereComponent* NewSphere) { InteractionSphere = NewSphere; }

	UFUNCTION(BlueprintCallable)
	bool TryAcquireClue(ADetectivePlayerState* PS) const;

	UFUNCTION(BlueprintCallable)
	void Interact(class ADetectivePlayerController* InController);
};
