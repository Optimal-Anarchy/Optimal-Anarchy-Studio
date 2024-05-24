// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ImGUINodeBase.h"
#include "MiscLibrary.h"
#include "ImGUIHUD.generated.h"

USTRUCT()
struct FImGUILink
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 StartNodeID = -1;

	UPROPERTY()
	int32 StartNodeAttributeID = -1;

	UPROPERTY()
	int32 EndNodeID = -1;

	UPROPERTY()
	int32 EndNodeAttributeID = -1;

	FImGUILink() {}

	FImGUILink(const int32 InStartNodeID, const int32 InStartNodeAttributeID, const int32 InEndNodeID, const int32 InEndNodeAttributeID)
	{
		StartNodeID = InStartNodeID;
		StartNodeAttributeID = InStartNodeAttributeID;
		EndNodeID = InEndNodeID;
		EndNodeAttributeID = InEndNodeAttributeID;
	}

};

USTRUCT()
struct FGraphSave
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY()
	TArray<FNodeSave> Nodes = TArray<FNodeSave>();

	UPROPERTY()
	TArray<FRegisteredNodeSave> RegisteredNodes = TArray<FRegisteredNodeSave>();

	UPROPERTY()
	TArray<FImGUILink> Links = TArray<FImGUILink>();

	FGraphSave() {}

	void Clear()
	{
		Nodes.Empty();
		RegisteredNodes.Empty();
		Links.Empty();
	}
};

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API AImGUIHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	static void RegisterNewNodeToHUD(class UImGUINodeBase* NodeToRegister);

	static void UnregisterNodeFromHUD(class UImGUINodeBase* NodeToUnregister);

	void BuildSaveStruct(FGraphSave& SaveStruct);

	void LoadSaveStruct(const FGraphSave& SaveStruct);

protected:

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UImGUIPopUpNode>> PopUpNodesAvailable;

	UPROPERTY(EditAnywhere)
	FColor NodesColor = FColor(30, 30, 30, 255);

private:

	FIDStore NodesIDStore;

	FIDStore AttributesIDStore;

	FIDStore LinksIDStore;

	TMap<int32, class UImGUINodeBase*> CurrentNodesContainedIDMap;

	TMap<int32, FImGUILink> LinksCreatedIDMap;

	TMap<int32, FImGUIScalarAttribute*> AttributesIDMap;

	void RemoveLink(const int32 LinkID);

	void RegisterNewLink(const FImGUILink& NewLink);

	void DeleteGraphCompletely();
};
