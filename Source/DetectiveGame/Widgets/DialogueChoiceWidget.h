// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../DetectiveStruct.h"
#include "DialogueChoiceWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChoiceSelected, int32, ChoiceIndex);
/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API UDialogueChoiceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Dialogue")
	class UButton* ChoiceButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Dialogue")
	class UTextBlock* ChoiceText;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FDialogueChoice ChoiceData;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	int32 ChoiceIndex;

public:

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueChoiceSelected OnDialogueChoiceSelected;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetupChoice(const FDialogueChoice InChoice, bool IsAvailable, int32 InChoiceIndex);

protected:

	UFUNCTION()
	void OnChoiceButtonClicked();
};
