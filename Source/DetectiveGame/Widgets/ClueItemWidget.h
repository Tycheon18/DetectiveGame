// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "../DetectiveStruct.h"
#include "ClueItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API UClueItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	

public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:

	UPROPERTY()
	class UClueInventoryWidget* ParentWidget;
	
	UPROPERTY()
	FDetectiveNode ClueData;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ClueID;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ClueName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ClueDescription;

public:

	UFUNCTION(BlueprintCallable, Category = "Clue Item")
	void SetClueDetails(FDetectiveNode Clue);

	UFUNCTION(BlueprintCallable)
	void SetParentWidget(UClueInventoryWidget* InParentWidget) { ParentWidget = InParentWidget; }
};
