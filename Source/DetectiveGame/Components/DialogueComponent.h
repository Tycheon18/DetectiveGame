// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "../DetectiveStruct.h"
#include "DialogueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestDialogueUI, UDialogueComponent*, DialogueComponent, ADetectivePlayerState*, PlayerState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueUpdated, FDialogueNode, CurrentNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DETECTIVEGAME_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	UDataTable* DialogueDataTable;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	TArray<FDialogueNode> DialogueDatabase;

	UPROPERTY()
	TMap<int32, int32> DialogueIndexMap;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	int32 CurrentNodeID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 StartNodeID = 1;

	UPROPERTY()
	class ADetectivePlayerState* CurrentPlayerState;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueUpdated OnDialogueUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueEnded OnDialogueEnded;

	static FOnRequestDialogueUI OnRequestDialogueUI;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void LoadDialogueFromDataTable();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void InitializeDialogueMap();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(ADetectivePlayerState* PlayerState);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	FDialogueNode GetCurrentNode();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ProcessNode(int32 NodeID);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ContinueNextDialogue(int32 InNodeID);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void CallNextDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SelectChoice(int32 ChoiceIndex, ADetectivePlayerState* PlayerState);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool IsChoiceAvailable(const FDialogueChoice& Choice, ADetectivePlayerState* PlayerState);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndDialogue();

protected:

	FDialogueNode* GetNodeByID(int32 NodeID);
};
