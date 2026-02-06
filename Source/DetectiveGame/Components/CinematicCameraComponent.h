// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraActor.h"
#include "CinematicCameraComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DETECTIVEGAME_API UCinematicCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCinematicCameraComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ========================== Camera Settings ==========================

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Camera")
	USceneComponent* CameraPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Camera")
	bool bLookAtPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Camera")
	bool bRotateOwnerToPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Camera", meta = (ClampMin = "0.1", ClampMax = "3.0"))
	float BlendTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Camera")
	float BlendExp = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Camera", meta = (ClampMin = "90.0", ClampMax = "720.0"))
	float RotationSpeed = 360.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Camera|Debug")
	bool bShowDebugCamera = true;

	// ========================== Runtime Variables ==========================

	UPROPERTY(BlueprintReadOnly, Category = "Cinematic Camera")
	ACameraActor* TempCamera;

	UPROPERTY()
	AActor* PreviousViewTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Cinematic Camera")
	bool bIsCameraActive = false;

	UPROPERTY()
	bool bIsRotating = false;


	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY()
	APlayerController* CachedPlayerController;

	// ========================== Functions ==========================

	UFUNCTION(BlueprintCallable, Category = "Cinematic Camera")
	void ActivateCamera();

	UFUNCTION(BlueprintCallable, Category = "Cinematic Camera")
	void DeactivateCamera();

protected:

	void CreateTempCamera();

	void UpdateCameraTransform();

	void StartRotateToPlayer();

	void RotateCameraPointToPlayer();

	void DrawDebugVisualization();
};
