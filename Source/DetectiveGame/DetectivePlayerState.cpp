// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectivePlayerState.h"
#include "DetectiveStruct.h"

void ADetectivePlayerState::BeginPlay()
{
	Super::BeginPlay();

	LoadCluesFromDataTable();
}

void ADetectivePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}

void ADetectivePlayerState::LoadCluesFromDataTable()
{
	if (!ClueDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[PlayerState] ClueDataTable not set!"));
		return;
	}

	ClueDatabase.Empty();

	TArray<FDetectiveNode*> AllRows;
	ClueDataTable->GetAllRows<FDetectiveNode>(TEXT("LoadClues"), AllRows);

	for (FDetectiveNode* Row : AllRows)
	{
		if (Row) ClueDatabase.Add(*Row);
	}

	UE_LOG(LogTemp, Log, TEXT("[PlayerState] Loaded %d clues"), ClueDatabase.Num());

	InitializeClueIndexMap();
}

void ADetectivePlayerState::InitializeClueIndexMap()
{
	ClueIndexMap.Empty();

	for (int32 i = 0; i < ClueDatabase.Num(); i++)
	{
		ClueIndexMap.Add(ClueDatabase[i].ID, i);
	}
}

void ADetectivePlayerState::UpdateClue(int32 ID)
{
	if (!ClueIndexMap.Contains(ID))
	{
		return;
	}

	int32 ClueIndex = ClueIndexMap[ID];
	FDetectiveNode& CurrentClue = ClueDatabase[ClueIndex];

	for (int32 NextClueID : CurrentClue.UnlocksNodes)
	{
		if (!ClueIndexMap.Contains(NextClueID))
		{
			continue;
		}

		int32 NextClueIndex = ClueIndexMap[NextClueID];
		FDetectiveNode& NextClue = ClueDatabase[NextClueIndex];
		NextClue.RequiredNodeChecks++;

		UE_LOG(LogTemp, Warning, TEXT("[PlayerState] Clue %d progress: %d/%d"), NextClueID, NextClue.RequiredNodeChecks, NextClue.RequiredNodes.Num());

		if ((NextClue.RequiredNodeChecks >= NextClue.RequiredNodes.Num()) && NextClue.bIsAutoActive)
		{
			ActivateClue(NextClueID);
		}
	}
}

void ADetectivePlayerState::ActivateClue(int32 ID)
{
	UE_LOG(LogTemp, Warning, TEXT("[PlayerState] Activate Clue %d is Called"), ID);

	if (!ClueIndexMap.Contains(ID)) return;

	int32 ClueIndex = ClueIndexMap[ID];
	FDetectiveNode& Clue = ClueDatabase[ClueIndex];

	if (Clue.bIsActive) return;

	Clue.bIsActive = true;

	UpdateClue(ID);
}

bool ADetectivePlayerState::IsClueActive(int32 ID) const
{
	if (!ClueIndexMap.Contains(ID))
	{
		return false;
	}

	int32 ClueIndex = ClueIndexMap[ID];

	if (!ClueDatabase.IsValidIndex(ClueIndex))
	{
		return false;
	}

	return ClueDatabase[ClueIndex].bIsActive;
}

bool ADetectivePlayerState::IsConditionMet(const TArray<int32>& Entry) const
{
	if (Entry.Num() == 0) return true;

	for (int32 ClueID : Entry)
	{
		if (!IsClueActive(ClueID))
		{
			return false;
		}
	}

	return true;
}

TArray<FDetectiveNode> ADetectivePlayerState::GetClueDatabase() const
{
	return ClueDatabase;
}
