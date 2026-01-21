// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueChoiceWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDialogueChoiceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ChoiceButton->OnClicked.AddDynamic(this, &UDialogueChoiceWidget::OnChoiceButtonClicked);
}

void UDialogueChoiceWidget::SetupChoice(const FDialogueChoice InChoice, bool IsAvailable, int32 InChoiceIndex)
{
	ChoiceIndex = InChoiceIndex;

	ChoiceText->SetText(InChoice.ChoiceText);

	if(IsAvailable)
	{
		ChoiceButton->SetIsEnabled(true);
	}
	else
	{
		ChoiceButton->SetIsEnabled(false);
	}
}

void UDialogueChoiceWidget::OnChoiceButtonClicked()
{
	OnDialogueChoiceSelected.Broadcast(ChoiceIndex);
}
