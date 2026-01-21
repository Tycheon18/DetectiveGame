// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"
#include "../DetectivePlayerState.h"
#include "../Actors/DetectiveNPC.h"
#include "Engine/DataTable.h"


FOnRequestDialogueUI UDialogueComponent::OnRequestDialogueUI;
// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	LoadDialogueFromDataTable();
}


// Called every frame
void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDialogueComponent::LoadDialogueFromDataTable()
{
    if (!DialogueDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[DialogueComponent] No DataTable assigned!"));
        return;
    }

    TArray<FDialogueNode*> AllRows;
    DialogueDataTable->GetAllRows<FDialogueNode>(TEXT("LoadDialogue"), AllRows);

    DialogueDatabase.Empty();
    for (FDialogueNode* Row : AllRows)
    {
        if (Row)
        {
            DialogueDatabase.Add(*Row);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[DialogueComponent] Loaded %d dialogue nodes from DataTable"), DialogueDatabase.Num());

	InitializeDialogueMap();
}

void UDialogueComponent::InitializeDialogueMap()
{
    DialogueIndexMap.Empty();

    for (int32 i = 0; i < DialogueDatabase.Num(); ++i)
    {
        DialogueIndexMap.Add(DialogueDatabase[i].NodeID, i);
    }

    UE_LOG(LogTemp, Log, TEXT("[DialogueComponent] Dialogue IndexMap initialized with %d entries"), DialogueIndexMap.Num());

}

void UDialogueComponent::StartDialogue(ADetectivePlayerState* PlayerState)
{
    if (!PlayerState)
    {
        UE_LOG(LogTemp, Warning, TEXT("[DialogueComponent] Invalid PlayerState "));
        return;
    }

	CurrentPlayerState = PlayerState;
    CurrentNodeID = StartNodeID;

	OnRequestDialogueUI.Broadcast(this, PlayerState);

    FDialogueNode CurrentNode = GetCurrentNode();
    OnDialogueUpdated.Broadcast(CurrentNode);

    UE_LOG(LogTemp, Log, TEXT("[DialogueComponent] Dialogue started at NodeID: %d"), CurrentNodeID);
}

FDialogueNode UDialogueComponent::GetCurrentNode()
{
    FDialogueNode* Node = GetNodeByID(CurrentNodeID);
	return Node ? *Node : FDialogueNode();
}

void UDialogueComponent::ProcessNode(int32 NodeID)
{
    FDialogueNode* Node = GetNodeByID(NodeID);

    if (!Node)
    {
        UE_LOG(LogTemp, Warning, TEXT("[DialogueComponent] NodeID %d not found in ProcessNode"), NodeID);
        EndDialogue();
        return;
	}

    CurrentNodeID = NodeID;
    OnDialogueUpdated.Broadcast(*Node);
    UE_LOG(LogTemp, Log, TEXT("[DialogueComponent] Processed NodeID: %d"), CurrentNodeID);
}

void UDialogueComponent::ContinueNextDialogue(int32 InNodeID)
{
    if(InNodeID == -1)
    {
        EndDialogue();
    }
    else
    {
        ProcessNode(InNodeID);
	}
}

void UDialogueComponent::CallNextDialogue()
{
	FDialogueNode CurrentNode = GetCurrentNode();

    if (CurrentNode.bIsAutoProgress)
    {
		int32 NextNodeID = CurrentNode.NextNodeID;

        if (NextNodeID == -1)
        {
            EndDialogue();
        }
		else
        {
            ProcessNode(CurrentNode.NextNodeID);
        }
    }
}

void UDialogueComponent::SelectChoice(int32 ChoiceIndex, class ADetectivePlayerState* PlayerState)
{
    if (!PlayerState)
    {
		UE_LOG(LogTemp, Warning, TEXT("[DialogueComponent] Invalid PlayerState in SelectChoice"));
        return;
    }

    FDialogueNode CurrentNode = GetCurrentNode();
    
    if (!CurrentNode.Choices.IsValidIndex(ChoiceIndex))
    {
		UE_LOG(LogTemp, Warning, TEXT("[DialogueComponent] Invalid choice index: %d"), ChoiceIndex);
    }

    FDialogueChoice& SelectedChoice = CurrentNode.Choices[ChoiceIndex];

    if (!IsChoiceAvailable(SelectedChoice, PlayerState))
    {
		UE_LOG(LogTemp, Warning, TEXT("[DialogueComponent] Choice not available (missing required clues)"));
        return;
    }

    if (SelectedChoice.RewardClueID > 0)
    {
        PlayerState->ActivateClue(SelectedChoice.RewardClueID);
        UE_LOG(LogTemp, Log, TEXT("[DialogueComponent] Rewarded Clue: %d"), SelectedChoice.RewardClueID);
    }

    int32 NextNodeID = SelectedChoice.NextNodeID;

	ContinueNextDialogue(NextNodeID);

    //if (NextNodeID == -1)
    //{
    //    EndDialogue();
    //}
    //else
    //{
    //    CurrentNodeID = NextNodeID;
    //    FDialogueNode NextNode = GetCurrentNode();
    //    OnDialogueUpdated.Broadcast(NextNode);

    //    UE_LOG(LogTemp, Log, TEXT("[DialogueComponent] Moved to NodeID: %d"), CurrentNodeID);
    //}
}

bool UDialogueComponent::IsChoiceAvailable(const FDialogueChoice& Choice, class ADetectivePlayerState* PlayerState)
{
    if(!PlayerState)
    {
        return false;
    }

    if(Choice.RequiredClueIDs.Num() == 0)
    {
        return true;
	}

	return PlayerState->IsConditionMet(Choice.RequiredClueIDs);
}

void UDialogueComponent::EndDialogue()
{
    CurrentNodeID = -1;
    OnDialogueEnded.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("[DialogueComponent] Dialogue ended"));
}

FDialogueNode* UDialogueComponent::GetNodeByID(int32 NodeID)
{
    int32* IndexPtr = DialogueIndexMap.Find(NodeID);
    if (IndexPtr && DialogueDatabase.IsValidIndex(*IndexPtr))
    {
        return &DialogueDatabase[*IndexPtr];
    }

    UE_LOG(LogTemp, Warning, TEXT("[DialogueComponent] Node not found: %d"), NodeID);
    return nullptr;
}
