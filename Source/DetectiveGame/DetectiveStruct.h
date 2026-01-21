// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DetectiveStruct.generated.h"

UENUM(BlueprintType)
enum class EGamePlayMode : uint8
{
	Single UMETA(DisplayName = "SinglePlayer"),
	Multi UMETA(DisplayName = "MultiPlayer")
};

UENUM(BlueprintType)
enum class ENodeType : uint8
{
	Clue       UMETA(DisplayName = "Clue"),
	Deduction  UMETA(DisplayName = "Deduction"),
	Question   UMETA(DisplayName = "Question")
};

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None            UMETA(DisplayName = "None"),
	ObjectHistory   UMETA(DisplayName = "Object History"),
	EmotionDetection UMETA(DisplayName = "Emotion Detection"),
	ManaResidue     UMETA(DisplayName = "Mana Residue"),
	General         UMETA(DisplayName = "General")
};

UENUM(BlueprintType)
enum class EDialogueState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Focused     UMETA(DisplayName = "Focused"),
	Talking     UMETA(DisplayName = "Talking"),
	Explaining  UMETA(DisplayName = "Explaining"),
	Angry       UMETA(DisplayName = "Angry")
};

USTRUCT(Atomic, BlueprintType)
struct FOptionalDescriptions
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Descriptions;

};

USTRUCT(Atomic, BlueprintType)
struct FClueStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ClueNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsChecked = false;
};

// Not only using clue , but also using on branch and scripts
USTRUCT(Atomic, BlueprintType)
struct FDetectiveNode : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FText ClueName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FText ClueDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsAutoActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsKey = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connection")
	int32 RequiredNodeChecks = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connection")
	TArray<int32> RequiredNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connection")
	TArray<int32> UnlocksNodes;
};

USTRUCT(Atomic, BlueprintType)
struct FSpecialDetectiveNode : public FDetectiveNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	EAbilityType AcquiredByAbility = EAbilityType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FString Category;
};

USTRUCT(BlueprintType)
struct FDialogueChoice
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText ChoiceText = FText::FromString("Default Choice");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 NextNodeID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<int32> RequiredClueIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 RewardClueID = 0;

	FDialogueChoice()
		: ChoiceText(FText::FromString("Default Choice"))
		, NextNodeID(-1)
		, RewardClueID(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FDialogueNode : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 NodeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bIsAutoProgress = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue",
		meta = (EditCondition = "bIsAutoProgress"))
	int32 NextNodeID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FDialogueChoice> Choices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	EDialogueState AnimationState = EDialogueState::Focused;

	FDialogueNode()
		: NodeID(0)
		, SpeakerName(FText::FromString("Unknown"))
		, DialogueText(FText::FromString("..."))
		, bIsAutoProgress(false)
		, NextNodeID(-1)
	{
	}
};


USTRUCT(BlueprintType)
struct FDeductionRecipe : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deduction")
	int32 RecipeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deduction")
	TArray<int32> RequiredClueIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deduction")
	FText ResultTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deduction")
	FText ResultDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deduction")
	int32 RewardClueID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deduction")
	bool bIsUsed = false;

	FDeductionRecipe()
		: RecipeID(0)
		, ResultTitle(FText::FromString("Unknown Result"))
		, ResultDescription(FText::FromString("..."))
		, RewardClueID(0)
		, bIsUsed(false)
	{
	}
};

/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API UDetectiveStruct : public UObject
{
	GENERATED_BODY()
	
};
