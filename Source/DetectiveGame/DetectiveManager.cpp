// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectiveManager.h"
#include "Engine/DataTable.h"

void UDetectiveManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Log, TEXT("DetectiveManager Initialized"));

	// 초기화 작업
	ActiveNodes.Empty();
}

void UDetectiveManager::Deinitialize()
{
	Super::Deinitialize();

	UE_LOG(LogTemp, Log, TEXT("DetectiveManager Deinitialized"));
}

//~=============================================================================
// 데이터 관리
//~=============================================================================

void UDetectiveManager::SetClueDataTable(UDataTable* InDataTable)
{
	if (!InDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetClueDataTable: Invalid DataTable"));
		return;
	}

	// 구조체 타입 체크
	if (InDataTable->GetRowStruct() != FDetectiveNode::StaticStruct())
	{
		UE_LOG(LogTemp, Error, TEXT("SetClueDataTable: DataTable must use FDetectiveNode struct"));
		return;
	}

	ClueDataTable = InDataTable;
	UE_LOG(LogTemp, Log, TEXT("ClueDataTable set successfully"));
}

//~=============================================================================
// 노드 활성화
//~=============================================================================

void UDetectiveManager::ActivateNode(int32 NodeID)
{
	// 이미 활성화됨
	if (ActiveNodes.Contains(NodeID))
	{
		UE_LOG(LogTemp, Warning, TEXT("Node %d already active"), NodeID);
		return;
	}

	// 노드 존재 확인
	FDetectiveNode NodeData;
	if (!GetNodeByID(NodeID, NodeData))
	{
		UE_LOG(LogTemp, Error, TEXT("Node %d not found in DataTable"), NodeID);
		return;
	}

	NodeData.bIsActive = true;

	// 재귀 방지를 위한 Set
	TSet<int32> ProcessedNodes;
	ActivateNodeInternal(NodeID, ProcessedNodes);
}

void UDetectiveManager::ActivateNodeInternal(int32 NodeID, TSet<int32>& ProcessedNodes)
{
	// 이미 처리됨
	if (ProcessedNodes.Contains(NodeID))
		return;

	ProcessedNodes.Add(NodeID);

	// 이미 활성화됨
	if (ActiveNodes.Contains(NodeID))
		return;

	// 노드 데이터 가져오기
	FDetectiveNode NodeData;
	if (!GetNodeByID(NodeID, NodeData))
		return;

	// 활성화
	ActiveNodes.Add(NodeID);
	UE_LOG(LogTemp, Log, TEXT("Node Activated: %d - %s"), NodeID, *NodeData.ClueName.ToString());

	// 연쇄 해금
	for (int32 UnlockID : NodeData.UnlocksNodes)
	{
		ActivateNodeInternal(UnlockID, ProcessedNodes);
	}

	// 다른 노드들의 조건 체크
	CheckAndActivateDependentNodes(NodeID);
}

void UDetectiveManager::CheckAndActivateDependentNodes(int32 ActivatedNodeID)
{
	TArray<FDetectiveNode> AllNodes = GetAllNodes();

	for (const FDetectiveNode& Node : AllNodes)
	{
		// bIsAutoActive가 True인 노드만 자동 활성화
		if (!Node.bIsAutoActive)
			continue;

		// 활성화 가능한지 체크
		if (CanActivateNode(Node.ID))
		{
			TSet<int32> TempProcessed;
			ActivateNodeInternal(Node.ID, TempProcessed);
		}
	}
}

bool UDetectiveManager::IsNodeActive(int32 NodeID) const
{
	return ActiveNodes.Contains(NodeID);
}

TArray<FDetectiveNode> UDetectiveManager::GetActiveNodes() const
{
	TArray<FDetectiveNode> Result;
	TArray<FDetectiveNode> AllNodes = GetAllNodes();

	for (const FDetectiveNode& Node : AllNodes)
	{
		if (ActiveNodes.Contains(Node.ID))
		{
			Result.Add(Node);
		}
	}

	// ID 순으로 정렬
	Result.Sort([](const FDetectiveNode& A, const FDetectiveNode& B) {
		return A.ID < B.ID;
		});

	return Result;
}

bool UDetectiveManager::GetNodeByID(int32 NodeID, FDetectiveNode& OutNode) const
{
	if (!ClueDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClueDataTable not set"));
		return false;
	}

	// Row Name으로 검색
	FString RowName = FString::FromInt(NodeID);
	FDetectiveNode* FoundNode = ClueDataTable->FindRow<FDetectiveNode>(FName(*RowName), TEXT("GetNodeByID"));

	if (!FoundNode)
	{
		// Row Name이 숫자가 아닐 수도 있으므로 전체 검색
		TArray<FDetectiveNode*> AllRows;
		ClueDataTable->GetAllRows(TEXT("GetNodeByID"), AllRows);

		for (FDetectiveNode* Row : AllRows)
		{
			if (Row && Row->ID == NodeID)
			{
				OutNode = *Row;
				return true;
			}
		}

		return false;
	}

	OutNode = *FoundNode;
	return true;
}

//~=============================================================================
// 추론 시스템 (자동)
//~=============================================================================

bool UDetectiveManager::CanActivateNode(int32 NodeID) const
{
	// 이미 활성화됨
	if (ActiveNodes.Contains(NodeID))
		return false;

	// 노드 데이터 가져오기
	FDetectiveNode NodeData;
	if (!GetNodeByID(NodeID, NodeData))
		return false;

	// RequiredNodes가 없으면 언제든 활성화 가능
	if (NodeData.RequiredNodes.Num() == 0)
		return true;

	// 모든 RequiredNodes가 활성화되었는지 확인
	for (int32 RequiredID : NodeData.RequiredNodes)
	{
		if (!ActiveNodes.Contains(RequiredID))
			return false;
	}

	return true;
}

//~=============================================================================
// 게임 진행
//~=============================================================================

void UDetectiveManager::ResetAllNodes()
{
	ActiveNodes.Empty();
	UE_LOG(LogTemp, Log, TEXT("All nodes reset"));
}

void UDetectiveManager::ActivateAutoNodes()
{
	if (!ClueDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActivateAutoNodes: ClueDataTable not set"));
		return;
	}

	TArray<FDetectiveNode> AllNodes = GetAllNodes();

	for (const FDetectiveNode& Node : AllNodes)
	{
		if (Node.bIsAutoActive)
		{
			ActivateNode(Node.ID);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Auto nodes activated: %d nodes"), ActiveNodes.Num());
}

//~=============================================================================
// Helper Functions
//~=============================================================================

TArray<FDetectiveNode> UDetectiveManager::GetAllNodes() const
{
	TArray<FDetectiveNode> Result;

	if (!ClueDataTable)
		return Result;

	TArray<FDetectiveNode*> AllRows;
	ClueDataTable->GetAllRows(TEXT("GetAllNodes"), AllRows);

	for (FDetectiveNode* Row : AllRows)
	{
		if (Row)
		{
			Result.Add(*Row);
		}
	}

	return Result;
}