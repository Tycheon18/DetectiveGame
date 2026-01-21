// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectiveGameModeBase.h"
#include "DetectivePlayerController.h"
#include "DetectivePlayerState.h"

void ADetectiveGameModeBase::BeginPlay()
{
	Super::BeginPlay();

    FTimerHandle InitTimer;
    GetWorld()->GetTimerManager().SetTimer(InitTimer, [this]()
        {
            for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
            {
                ADetectivePlayerController* PC = Cast<ADetectivePlayerController>(It->Get());
                if (PC)
                {
                    ADetectivePlayerState* PS = PC->GetPlayerState<ADetectivePlayerState>();
                    if (PS)
                    {
                        PS->InitializeClueIndexMap();
                        UE_LOG(LogTemp, Warning, TEXT("[GameModeBase] Initialized ClueMap for %s"), *PS->GetName());
                    }
                }
            }
        }, 1.0f, false);
}
