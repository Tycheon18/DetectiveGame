// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../DetectiveStruct.h"
#include "DetectiveNPC.generated.h"

UCLASS()
class DETECTIVEGAME_API ADetectiveNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADetectiveNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	class UDialogueComponent* DialogueComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	class UInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	class UCinematicCameraComponent* CinematicCameraComponent;

protected:
	/** 대화 시작 시 자동으로 카메라 활성화 */
	UFUNCTION()
	void OnDialogueStarted(AActor* InActor);

	/** 대화 종료 시 자동으로 카메라 비활성화 */
	UFUNCTION()
	void OnDialogueFinished();
};
