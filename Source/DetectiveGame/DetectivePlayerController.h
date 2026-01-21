// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DetectivePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API ADetectivePlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	virtual void BeginPlay() override;

protected:

	bool bIsInteracting = false;

	class UInteractionComponent* InteractionComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	TSubclassOf<class UClueInventoryWidget> ClueInventoryWidgetClass;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DialogueWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentDialogueWidget;

public:

	UFUNCTION(BlueprintCallable, Category = "Detective")
	void Interaction();

	UFUNCTION(BlueprintCallable, Category = "Detective")
	void Deduction();

	UFUNCTION(BlueprintCallable, Category = "Detective")
	void Question();

	UFUNCTION(BlueprintCallable)
	UInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	UFUNCTION(BlueprintCallable)
	void SetInteractionComponent(UInteractionComponent* NewComponent) { InteractionComponent = NewComponent; }

	UFUNCTION()
	void HandleDialogueUIRequest(class UDialogueComponent* DialogueComponent, class ADetectivePlayerState* InPlayerState);

	UFUNCTION()
	void HandleDialogueEnded();

	UFUNCTION(Exec, Category = "Debug")
	void DebugActivateClue(int32 ClueID);

	UFUNCTION(Exec, Category = "Debug")
	void DebugShowClues();

	UFUNCTION(Exec, Category = "Debug")
	void DebugShowInventory();

	UFUNCTION(Exec, Category = "Debug")
	void DebugTestDialogue(int32 StartNode = 1);


};
