// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueItemWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "../DetectivePlayerController.h"
#include "../DetectivePlayerState.h"
#include "../DetectiveStruct.h"
#include "../ClueDataObject.h"
#include "Components/TextBlock.h"

void UClueItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UClueDataObject* ClueObject = Cast<UClueDataObject>(ListItemObject);

	if(ClueObject)
	{
		SetClueDetails(ClueObject->ClueData);
		SetParentWidget(ClueObject->OwningWidget);
	}

}

void UClueItemWidget::SetClueDetails(FDetectiveNode Clue)
{
	ClueData = Clue;

	if (ClueID)
	{
		ClueID->SetText(FText::AsNumber(Clue.ID));
	}

	if(ClueName)
	{
		ClueName->SetText(Clue.ClueName);
	}

	if (ClueDescription)
	{
		ClueDescription->SetText(Clue.ClueDescription);
	}

}
