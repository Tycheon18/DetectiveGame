// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectivePlayerController.h"
#include "DetectivePlayerState.h"
#include "Components/InteractionComponent.h"
#include "Components/DialogueComponent.h"
#include "Widgets/ClueInventoryWidget.h"
#include "Widgets/DialogueWidget.h"

void ADetectivePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UDialogueComponent::OnRequestDialogueUI.AddDynamic(this, &ADetectivePlayerController::HandleDialogueUIRequest);
}

void ADetectivePlayerController::HandleDialogueUIRequest(UDialogueComponent* DialogueComponent, ADetectivePlayerState* InPlayerState)
{
	if (!DialogueWidgetClass || !DialogueComponent || !InPlayerState) return;

	if (CurrentDialogueWidget)
	{
		CurrentDialogueWidget->RemoveFromParent();
		CurrentDialogueWidget = nullptr;
	}

	CurrentDialogueWidget = CreateWidget<UUserWidget>(this, DialogueWidgetClass);
	
	UDialogueWidget* DialogueWidget = Cast<UDialogueWidget>(CurrentDialogueWidget);

	if (DialogueWidget)
	{
		DialogueWidget->AddToViewport();
		
		DialogueWidget->SetupDialogue(DialogueComponent, InPlayerState);
		//UFunction* InitFunc = CurrentDialogueWidget->FindFunction(FName("InitializeDialogue"));

		//if (InitFunc)
		//{
		//	struct FParams
		//	{
		//		UDialogueComponent* DialogueComp;
		//		ADetectivePlayerState* PlayerStateRef;
		//	};

		//	FParams Params;
		//	Params.DialogueComp = DialogueComponent;
		//	Params.PlayerStateRef = InPlayerState;

		//	DialogueWidget->ProcessEvent(InitFunc, &Params);
		//}


		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;

		DialogueComponent->OnDialogueEnded.RemoveDynamic(
			this, &ADetectivePlayerController::HandleDialogueEnded
		);
		DialogueComponent->OnDialogueEnded.AddDynamic(this, &ADetectivePlayerController::HandleDialogueEnded);
	}
}

void ADetectivePlayerController::HandleDialogueEnded()
{
	if(CurrentDialogueWidget)
	{
		CurrentDialogueWidget->RemoveFromParent();
		CurrentDialogueWidget = nullptr;
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void ADetectivePlayerController::DebugActivateClue(int32 ClueID)
{
	UE_LOG(LogTemp, Warning, TEXT("[PlayerController] Debug Activate Clue %d is Called"), ClueID);
	ADetectivePlayerState* PS = GetPlayerState<ADetectivePlayerState>();
	if (PS) PS->ActivateClue(ClueID);
}

void ADetectivePlayerController::DebugShowClues()
{
	ADetectivePlayerState* PS = GetPlayerState<ADetectivePlayerState>();
	if (!PS) return;

	for (const FDetectiveNode& Clue : PS->GetClueDatabase())
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] %d: %s"),
			Clue.bIsActive ? TEXT("O") : TEXT(" "),
			Clue.ID, *Clue.ClueName.ToString());
	}
}

void ADetectivePlayerController::DebugShowInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("[PlayerController] Debug Show Inventory is Called"));

	if (ClueInventoryWidgetClass)
	{
		UClueInventoryWidget* InventoryWidget = CreateWidget<UClueInventoryWidget>(this, ClueInventoryWidgetClass);
		if (InventoryWidget)
		{
			ADetectivePlayerState* PS = GetPlayerState<ADetectivePlayerState>();
			InventoryWidget->InitializeWidget(this, PS);
			InventoryWidget->SetClueStruct(PS->GetClueDatabase());
			InventoryWidget->RefreshClueLists();

			InventoryWidget->AddToViewport();

			SetShowMouseCursor(true);

			SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(InventoryWidget->TakeWidget()).SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock));
		}
	}
}

void ADetectivePlayerController::DebugTestDialogue(int32 StartNode)
{
	UE_LOG(LogTemp, Warning, TEXT("[PlayerController] Debug Test Dialogue is Called with StartNode: %d"), StartNode);
}

void ADetectivePlayerController::Interaction()
{
	if (InteractionComponent)
	{
		InteractionComponent->Interact(this);
	}
}

void ADetectivePlayerController::Deduction()
{
}

void ADetectivePlayerController::Question()
{

}