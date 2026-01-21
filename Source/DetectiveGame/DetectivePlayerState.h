// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DetectiveStruct.h"
#include "DetectivePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API ADetectivePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detective|Data")
	UDataTable* ClueDataTable;
	
	TMap<int32, int32> ClueIndexMap;

	TArray<FDetectiveNode> ClueDatabase;


public:

	void LoadCluesFromDataTable();

	void InitializeClueIndexMap();

	UFUNCTION(BlueprintCallable)
	void UpdateClue(int32 ID);

	UFUNCTION(BlueprintCallable)
	void ActivateClue(int32 ID);

	UFUNCTION(BlueprintCallable)
	bool IsClueActive(int32 ID) const;

	UFUNCTION(BlueprintCallable, Category = "Condition")
	bool IsConditionMet(const TArray<int32>& Entry) const;

	UFUNCTION(BlueprintCallable)
	TArray<FDetectiveNode> GetClueDatabase() const;

};
