// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DetectiveStruct.h"
#include "ClueDataObject.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API UClueDataObject : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly)
	class UClueInventoryWidget* OwningWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Clue")
	FDetectiveNode ClueData;

	UFUNCTION(BlueprintCallable, Category = "Clue")
	void InitializeClueData(const FDetectiveNode& NewClueData)
	{
		ClueData = NewClueData;
	}

	UFUNCTION(BlueprintPure, Category = "Clue")
	int32 GetClueID() const
	{
		return ClueData.ID;
	}

	UFUNCTION(BlueprintPure, Category = "Clue")
	FText GetClueName() const
	{
		return ClueData.ClueName;
	}

	UFUNCTION(BlueprintPure, Category = "Clue")
	FText GetClueDescription() const
	{
		return ClueData.ClueDescription;
	}

	UFUNCTION(BlueprintPure, Category = "Clue")
	bool IsClueActive() const
	{
		return ClueData.bIsActive;
	}
};
