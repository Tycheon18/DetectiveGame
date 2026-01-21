// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClueInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API UClueInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	void InitializeWidget(class ADetectivePlayerController* InPlayerController, class ADetectivePlayerState* InPlayerState);

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	void RefreshClueLists();

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	void SetClueStruct(const TArray<FDetectiveNode>& NewClueStructs);

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	FDetectiveNode GetClueByID(int32 ClueID) const;

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	int32 GetActiveClueCount() const;

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	int32 GetCategoryClueCount(int32 CategoryMin, int32 CategoryMax) const;

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	void UpdateSceneListView(TArray<UObject*> NewItems);

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	void UpdateDeductionListView(TArray<UObject*> NewItems);

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	void UpdateInterviewListView(TArray<UObject*> NewItems);

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	void UpdateDoubtListView(TArray<UObject*> NewItems);

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	void UpdateConclusionListView(TArray<UObject*> NewItems);

	UFUNCTION(BlueprintCallable, Category = "Clue Inventory")
	void UpdateTotalClueCount(int32 ActiveCount, int32 TotalCount);



protected:

	UPROPERTY()
	ADetectivePlayerController* OwningPlayerController;

	UPROPERTY()
	ADetectivePlayerState* OwningPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "Clue Inventory")
	TArray<FDetectiveNode> ClueDatabase;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UListView* SceneListView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UListView* DeductionListView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UListView* InterviewListView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UListView* DoubtListView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UListView* ConclusionListView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TitleText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CountText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CloseButton;


protected:

	UPROPERTY(BlueprintReadOnly, Category = "Clue Inventory")
	TArray<UObject*> SceneClueObjects;

	UPROPERTY(BlueprintReadOnly, Category = "Clue Inventory")
	TArray<UObject*> DeductionClueObjects;

	UPROPERTY(BlueprintReadOnly, Category = "Clue Inventory")
	TArray<UObject*> InterviewClueObjects;

	UPROPERTY(BlueprintReadOnly, Category = "Clue Inventory")
	TArray<UObject*> DoubtClueObjects;

	UPROPERTY(BlueprintReadOnly, Category = "Clue Inventory")
	TArray<UObject*> ConclusionClueObjects;

	UFUNCTION(BlueprintImplementableEvent, Category = "Clue Inventory")
	void UpdateCategoryCounts(int32 SceneCount, int32 DeductionCount, int32 InterviewCount, int32 DoubtCount, int32 ConclusionCount);


protected:

	int32 GetClueCategory(int32 ClueID) const;

	class UClueDataObject* CreateClueObject(const FDetectiveNode& NewClue);

	UFUNCTION(BlueprintCallable)
	void OnCloseButtonClicked();
};
