// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DetectiveStruct.h"
#include "DetectiveManager.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API UDetectiveManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	//~=============================================================================
	// 데이터 관리
	//~=============================================================================

	/** 단서 DataTable 설정 */
	UFUNCTION(BlueprintCallable, Category = "Detective|Data")
	void SetClueDataTable(UDataTable* InDataTable);

	/** 현재 DataTable 가져오기 */
	UFUNCTION(BlueprintPure, Category = "Detective|Data")
	UDataTable* GetClueDataTable() const { return ClueDataTable; }

	//~=============================================================================
	// 노드 활성화
	//~=============================================================================

	/** 노드 활성화 (연쇄 해금 포함) */
	UFUNCTION(BlueprintCallable, Category = "Detective|Node")
	void ActivateNode(int32 NodeID);

	/** 노드 활성화 여부 확인 */
	UFUNCTION(BlueprintPure, Category = "Detective|Node")
	bool IsNodeActive(int32 NodeID) const;

	/** 활성화된 모든 노드 가져오기 */
	UFUNCTION(BlueprintPure, Category = "Detective|Node")
	TArray<FDetectiveNode> GetActiveNodes() const;

	/** 활성화된 노드 ID 목록 */
	UFUNCTION(BlueprintPure, Category = "Detective|Node")
	TSet<int32> GetActiveNodeIDs() const { return ActiveNodes; }

	/** ID로 노드 데이터 가져오기 */
	UFUNCTION(BlueprintPure, Category = "Detective|Node")
	bool GetNodeByID(int32 NodeID, FDetectiveNode& OutNode) const;

	//~=============================================================================
	// 추론 시스템 (자동)
	//~=============================================================================

	/** 특정 노드가 해금 가능한지 체크 */
	UFUNCTION(BlueprintPure, Category = "Detective|Node")
	bool CanActivateNode(int32 NodeID) const;

	//~=============================================================================
	// 게임 진행
	//~=============================================================================

	/** 모든 노드 초기화 */
	UFUNCTION(BlueprintCallable, Category = "Detective|Game")
	void ResetAllNodes();

	/** 게임 시작 시 자동 활성화 노드들 활성화 */
	UFUNCTION(BlueprintCallable, Category = "Detective|Game")
	void ActivateAutoNodes();

	/** 활성 노드 개수 */
	UFUNCTION(BlueprintPure, Category = "Detective|Game")
	int32 GetActiveNodeCount() const { return ActiveNodes.Num(); }

protected:
	/** 단서 DataTable */
	UPROPERTY()
	UDataTable* ClueDataTable;

	/** 활성화된 노드 ID들 */
	UPROPERTY()
	TSet<int32> ActiveNodes;

	/** 노드 활성화 내부 로직 (재귀 방지) */
	void ActivateNodeInternal(int32 NodeID, TSet<int32>& ProcessedNodes);

	/** 모든 노드 순회하며 조건 체크 */
	void CheckAndActivateDependentNodes(int32 ActivatedNodeID);

	/** DataTable에서 모든 노드 가져오기 */
	TArray<FDetectiveNode> GetAllNodes() const;
};
