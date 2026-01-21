// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueInventoryWidget.h"
#include "../DetectivePlayerController.h"
#include "../DetectivePlayerState.h"
#include "../DetectiveStruct.h"
#include "../ClueDataObject.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UClueInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UClueInventoryWidget::OnCloseButtonClicked);

	UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] NativeConstruct Called"));
}

void UClueInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	SceneClueObjects.Empty();
	DeductionClueObjects.Empty();
	InterviewClueObjects.Empty();
	DoubtClueObjects.Empty();
	ConclusionClueObjects.Empty();

	UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] NativeDestruct Called"));
}

void UClueInventoryWidget::InitializeWidget(ADetectivePlayerController* InPlayerController, ADetectivePlayerState* InPlayerState)
{
	OwningPlayerController = InPlayerController;
	OwningPlayerState = InPlayerState;

	if(OwningPlayerState)
	{
		ClueDatabase = OwningPlayerState->GetClueDatabase();
		UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] InitializeWidget Called"));
	}
}

void UClueInventoryWidget::RefreshClueLists()
{
	if (!OwningPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ClueInventoryWidget] RefreshClueList: No PlayerState!"));
		return;
	}

	ClueDatabase = OwningPlayerState->GetClueDatabase();

	SceneClueObjects.Empty();
	DeductionClueObjects.Empty();
	InterviewClueObjects.Empty();
	DoubtClueObjects.Empty();
	ConclusionClueObjects.Empty();

	int32 ActiveCount = 0;
	int32 SceneCount = 0;
	int32 DeductionCount = 0;
	int32 InterviewCount = 0;
	int32 DoubtCount = 0;
	int32 ConclusionCount = 0;

	for (const FDetectiveNode& Clue : ClueDatabase)
	{
		if (!Clue.bIsActive) continue;

		ActiveCount++;

		UClueDataObject* ClueObject = CreateClueObject(Clue);
		if (!ClueObject) continue;

		int32 Category = GetClueCategory(Clue.ID);

		switch (Category)
		{
			case 1:
				SceneClueObjects.Add(ClueObject);
				SceneCount++;
				UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] Scene: [%d] %s"),
					Clue.ID, *Clue.ClueName.ToString());
				break;

			case 2:
				DeductionClueObjects.Add(ClueObject);
				DeductionCount++;
				UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] Deduction: [%d] %s"),
					Clue.ID, *Clue.ClueName.ToString());
				break;

			case 3:
				InterviewClueObjects.Add(ClueObject);
				InterviewCount++;
				UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] Interview: [%d] %s"),
					Clue.ID, *Clue.ClueName.ToString());
				break;

			case 4:
				DoubtClueObjects.Add(ClueObject);
				DoubtCount++;
				UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] Doubt: [%d] %s"),
					Clue.ID, *Clue.ClueName.ToString());
				break;

			case 5:
				ConclusionClueObjects.Add(ClueObject);
				ConclusionCount++;
				UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] Conclusion: [%d] %s"),
					Clue.ID, *Clue.ClueName.ToString());
				break;
		}
	}

	UpdateSceneListView(SceneClueObjects);
	UpdateDeductionListView(DeductionClueObjects);
	UpdateInterviewListView(InterviewClueObjects);
	UpdateDoubtListView(DoubtClueObjects);
	UpdateConclusionListView(ConclusionClueObjects);

	UpdateCategoryCounts(SceneCount, DeductionCount, InterviewCount, DoubtCount, ConclusionCount);
	UpdateTotalClueCount(ActiveCount, ClueDatabase.Num());

	UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] Refresh complete: %d/%d active clues"),
		ActiveCount, ClueDatabase.Num());
	UE_LOG(LogTemp, Log, TEXT("  Scene: %d, Deduction: %d, Interview: %d, Doubt: %d, Conclusion: %d"),
		SceneCount, DeductionCount, InterviewCount, DoubtCount, ConclusionCount);
}

void UClueInventoryWidget::SetClueStruct(const TArray<FDetectiveNode>& NewClueStructs)
{
	ClueDatabase = NewClueStructs;
	UE_LOG(LogTemp, Log, TEXT("[ClueInventoryWidget] ClueDatabase updated: %d clues"), ClueDatabase.Num());
}

FDetectiveNode UClueInventoryWidget::GetClueByID(int32 ClueID) const
{
	for (const FDetectiveNode& Clue : ClueDatabase)
	{
		if (Clue.ID == ClueID)
		{
			return Clue;
		}
	}

	return FDetectiveNode();
}

int32 UClueInventoryWidget::GetActiveClueCount() const
{
	int32 Count = 0;

	for(const FDetectiveNode& Clue : ClueDatabase)
	{
		if (Clue.bIsActive)
		{
			Count++;
		}
	}

	return Count;
}

int32 UClueInventoryWidget::GetCategoryClueCount(int32 CategoryMin, int32 CategoryMax) const
{
	int32 Count = 0;

	for (const FDetectiveNode& Clue : ClueDatabase)
	{
		if (Clue.bIsActive && Clue.ID >= CategoryMin && Clue.ID <= CategoryMax)
		{
			Count++;
		}
	}

	return Count;
}

void UClueInventoryWidget::UpdateSceneListView(TArray<UObject*> NewItems)
{
	SceneListView->ClearListItems();
	SceneListView->SetListItems(NewItems);
}

void UClueInventoryWidget::UpdateDeductionListView(TArray<UObject*> NewItems)
{
	DeductionListView->ClearListItems();
	DeductionListView->SetListItems(NewItems);
}

void UClueInventoryWidget::UpdateInterviewListView(TArray<UObject*> NewItems)
{
	InterviewListView->ClearListItems();
	InterviewListView->SetListItems(NewItems);
}

void UClueInventoryWidget::UpdateDoubtListView(TArray<UObject*> NewItems)
{
	DoubtListView->ClearListItems();
	DoubtListView->SetListItems(NewItems);
}

void UClueInventoryWidget::UpdateConclusionListView(TArray<UObject*> NewItems)
{
	ConclusionListView->ClearListItems();
	ConclusionListView->SetListItems(NewItems);
}

void UClueInventoryWidget::UpdateTotalClueCount(int32 ActiveCount, int32 TotalCount)
{
	if (CountText)
	{
		FString CountString = FString::Printf(TEXT("Clues: %d / %d"), ActiveCount, TotalCount);
		CountText->SetText(FText::FromString(CountString));
	}
}

int32 UClueInventoryWidget::GetClueCategory(int32 ClueID) const
{
	if (ClueID >= 1 && ClueID < 100)
		return 1; // Scene Clues
	else if (ClueID >= 100 && ClueID < 200)
		return 2; // Deduction Clues
	else if (ClueID >= 200 && ClueID < 300)
		return 3; // Interview Clues
	else if (ClueID >= 300 && ClueID < 400)
		return 4; // Doubt Clues
	else if (ClueID >= 400)
		return 5; // Conclusion Clues

	return 0; // Unknown Category
}

UClueDataObject* UClueInventoryWidget::CreateClueObject(const FDetectiveNode& NewClue)
{
	UClueDataObject* ClueObject = NewObject<UClueDataObject>(this);

	if(ClueObject)
	{
		ClueObject->InitializeClueData(NewClue);
	}

	return ClueObject;
}

void UClueInventoryWidget::OnCloseButtonClicked()
{
	RemoveFromParent();
	if (OwningPlayerController)
	{
		OwningPlayerController->SetShowMouseCursor(false);
		OwningPlayerController->SetInputMode(FInputModeGameOnly());
	}
}