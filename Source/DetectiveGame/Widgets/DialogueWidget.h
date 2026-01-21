// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../DetectiveStruct.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(EditAnywhere,	BlueprintReadWrite, meta = (BindWidget), Category = "Dialogue")
	class UBorder* DialogueArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Dialogue")
	class UTextBlock* SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Dialogue")
	UTextBlock* DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Dialogue")
	class UButton* CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Dialogue")
	class UVerticalBox* ChoicesBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	class UDialogueComponent* DialogueComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	class ADetectivePlayerState* PlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	class UUserWidget* ChoiceButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TSubclassOf<class UDialogueChoiceWidget> DialogueChoiceWidgetClass;
public:

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetupDialogue(class UDialogueComponent* InDialogueComponent, class ADetectivePlayerState* InPlayerState);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void UpdateDialogue(const FDialogueNode CurrentNode);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void HandleChoiceSelected(int32 ChoiceIndex);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void CloseDialogue();

	UFUNCTION()
	void OnDialogueAreaClicked();
};
