// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "DialogueChoiceWidget.h"
#include "../Components/DialogueComponent.h"
#include "../DetectivePlayerState.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// You can bind button click events here if needed
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UDialogueWidget::CloseDialogue);
	}
}

void UDialogueWidget::SetupDialogue(UDialogueComponent* InDialogueComponent, ADetectivePlayerState* InPlayerState)
{
	DialogueComponent = InDialogueComponent;
	PlayerState = InPlayerState;

	DialogueComponent->OnDialogueUpdated.AddDynamic(this, &UDialogueWidget::UpdateDialogue);
	DialogueComponent->OnDialogueEnded.AddDynamic(this, &UDialogueWidget::CloseDialogue);

}

void UDialogueWidget::UpdateDialogue(const FDialogueNode CurrentNode)
{
	SpeakerName->SetText(CurrentNode.SpeakerName);
	DialogueText->SetText(CurrentNode.DialogueText);

	ChoicesBox->ClearChildren();

	if(!CurrentNode.bIsAutoProgress)
	{
		int32 ChoiceCount = 0;

		for (const FDialogueChoice& Choice : CurrentNode.Choices)
		{
			UDialogueChoiceWidget* ChoiceWidget = CreateWidget<UDialogueChoiceWidget>(GetWorld(), DialogueChoiceWidgetClass);

			if (ChoiceWidget)
			{
				bool bIsAvailable = DialogueComponent->IsChoiceAvailable(Choice, PlayerState);
				ChoiceWidget->SetupChoice(Choice, bIsAvailable, ChoiceCount);
				ChoiceWidget->OnDialogueChoiceSelected.AddDynamic(this, &UDialogueWidget::HandleChoiceSelected);
				ChoicesBox->AddChild(ChoiceWidget);
			}

			ChoiceCount++;
		}
	}
	else
	{
		DialogueArea->OnMouseButtonDownEvent.BindUFunction(this, FName("OnDialogueAreaClicked"));
	}
}

void UDialogueWidget::HandleChoiceSelected(int32 ChoiceIndex)
{
	DialogueComponent->SelectChoice(ChoiceIndex, PlayerState);
}

void UDialogueWidget::CloseDialogue()
{
	RemoveFromParent();
}

void UDialogueWidget::OnDialogueAreaClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("[DialogueWidget] Dialogue area clicked, calling next dialogue."));
	
	DialogueComponent->CallNextDialogue();
	
}
