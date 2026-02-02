// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DetectiveInteractable.generated.h"

UCLASS()
class DETECTIVEGAME_API ADetectiveInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADetectiveInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	class UInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	class USphereComponent* InteractionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective")
	class UCinematicCameraComponent* CinematicCameraComponent;

	/** 인터랙션 시 카메라 클로즈업 사용 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective|Camera")
	bool bUseCloseUpCamera = true;

	/** 클로즈업 지속 시간 (초, 0이면 수동 해제) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detective|Camera", meta = (EditCondition = "bUseCloseUpCamera"))
	float CloseUpDuration = 2.0f;

protected:
	/** 인터랙션 시 카메라 클로즈업 */
	UFUNCTION()
	void OnInteracted(AActor* InActor);

	/** 타이머로 카메라 자동 복귀 */
	FTimerHandle CloseUpTimerHandle;
	void DeactivateCloseUpCamera();
};
