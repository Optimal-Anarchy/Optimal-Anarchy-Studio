// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUIHUD.h"
#include "ImGUIPopUpNode.h"
#include "BiomiPlugin/BiomiPlugin.h"
#include "ImGuiModule.h"
#include "Kismet/GameplayStatics.h"


void AImGUIHUD::RegisterNewNodeToHUD(UImGUINodeBase* NodeToRegister)
{
	if (AImGUIHUD* HUDRef = Cast<AImGUIHUD>(NodeToRegister->GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		TArray<int32> NewNodeAttributesID;
		NewNodeAttributesID.SetNum(NodeToRegister->GetTotalNumberOfAttributes());
		for (int32 i = 0; i < NewNodeAttributesID.Num(); i++)
		{
			NewNodeAttributesID[i] = HUDRef->AttributesIDStore.TakeNewID();
		}
		NodeToRegister->InitiateNode(HUDRef->NodesIDStore.TakeNewID(), NewNodeAttributesID);
		HUDRef->CurrentNodesContainedIDMap.Add(NodeToRegister->NodeID, NodeToRegister);
		for (int32 i = 0; i < NodeToRegister->InputAttributes.Num(); ++i)
		{
			HUDRef->AttributesIDMap.Add(NodeToRegister->InputAttributes[i].AttributeID, &NodeToRegister->InputAttributes[i]);
		}
		for (int32 i = 0; i < NodeToRegister->OutputAttributes.Num(); ++i)
		{
			HUDRef->AttributesIDMap.Add(NodeToRegister->OutputAttributes[i].AttributeID, &NodeToRegister->OutputAttributes[i]);
		}

		int32 ViewportSizeX;
		int32 ViewportSizeY;
		HUDRef->GetOwningPlayerController()->GetViewportSize(ViewportSizeX, ViewportSizeY);
		ImNodes::SetNodeGridSpacePos(NodeToRegister->NodeID, ImVec2(FMath::RandRange(0.f, float(ViewportSizeX / 1.5f - NodeToRegister->WindowSize.X)),
			FMath::RandRange(0.f, float(ViewportSizeY / 1.5f - NodeToRegister->WindowSize.Y))));
	}
	else
	{
		UE_LOG(GeneralImGUILog, Warning, TEXT("Ensure that the HUD class is a AImGUIHUD, see AImGUIHUD::RegisterNewNodeToHUD"));
	}
}

void AImGUIHUD::UnregisterNodeFromHUD(class UImGUINodeBase* NodeToUnregister)
{
	if (NodeToUnregister->GetWorld() && NodeToUnregister->GetWorld()->GetFirstPlayerController())
	{
		if (AImGUIHUD* HUDRef = Cast<AImGUIHUD>(NodeToUnregister->GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			for (int32 y = NodeToUnregister->LinksID.Num() - 1; y >= 0; --y)
			{
				if (HUDRef->LinksCreatedIDMap.Contains(NodeToUnregister->LinksID[y]))
				{
					HUDRef->RemoveLink(NodeToUnregister->LinksID[y]);
				}
			}
			HUDRef->CurrentNodesContainedIDMap.Remove(NodeToUnregister->NodeID);
			for (int32 y = 0; y < NodeToUnregister->InputAttributes.Num(); ++y)
			{
				HUDRef->AttributesIDMap.Remove(NodeToUnregister->InputAttributes[y].AttributeID);
				HUDRef->AttributesIDStore.ReleaseID(NodeToUnregister->InputAttributes[y].AttributeID);
			}
			for (int32 y = 0; y < NodeToUnregister->OutputAttributes.Num(); ++y)
			{
				HUDRef->AttributesIDMap.Remove(NodeToUnregister->OutputAttributes[y].AttributeID);
				HUDRef->AttributesIDStore.ReleaseID(NodeToUnregister->OutputAttributes[y].AttributeID);
			}
			HUDRef->NodesIDStore.ReleaseID(NodeToUnregister->NodeID);
			ImNodes::ClearNodeSelection(NodeToUnregister->NodeID);
		}
	}
}

void AImGUIHUD::BeginPlay()
{
	Super::BeginPlay();

	FImGuiModule::Get().GetProperties().SetInputEnabled(true);
	FImGuiModule::Get().GetProperties().SetMouseInputShared(true);

	// NODE COLOR STYLE
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(NodesColor.R, NodesColor.G, NodesColor.B, NodesColor.A));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(NodesColor.R, NodesColor.G, NodesColor.B, NodesColor.A));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(NodesColor.R, NodesColor.G, NodesColor.B, NodesColor.A));
	ImNodes::PushColorStyle(ImNodesCol_NodeBackground, IM_COL32(NodesColor.R, NodesColor.G, NodesColor.B, NodesColor.A));
	ImNodes::PushColorStyle(ImNodesCol_NodeBackgroundHovered, IM_COL32(NodesColor.R, NodesColor.G, NodesColor.B, NodesColor.A));
	ImNodes::PushColorStyle(ImNodesCol_NodeBackgroundSelected, IM_COL32(NodesColor.R, NodesColor.G, NodesColor.B, NodesColor.A));
}

void AImGUIHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int32 ViewportSizeX;
	int32 ViewportSizeY;
	GetOwningPlayerController()->GetViewportSize(ViewportSizeX, ViewportSizeY);
	ImGui::SetNextWindowSize(ImVec2(ViewportSizeX / 1.5f, ViewportSizeY / 1.5f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(ViewportSizeX / 6.f, ViewportSizeY / 6.f), ImGuiCond_FirstUseEver);
	ImGui::Begin("Node Graph Editor",NULL,ImGuiWindowFlags_NoBackground);

	ImNodes::BeginNodeEditor();

	// APPLY MODIFIERS

	ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
	ImNodes::GetIO().MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyAlt;

    // POPUP

	if (PopUpNodesAvailable.Num() > 0 && !ImGui::IsAnyItemHovered() &&
		ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
		ImNodes::IsEditorHovered() && ImGui::GetIO().MouseClicked[1])
	{
		ImGui::OpenPopup("add node");
	}
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
    if (ImGui::BeginPopup("add node"))
    {
        const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

		for (int32 i = 0; i < PopUpNodesAvailable.Num(); ++i)
		{
			if (ImGui::MenuItem(TCHAR_TO_ANSI(*PopUpNodesAvailable[i].GetDefaultObject()->PopUpGenericName.ToString())))
			{
				UImGUIPopUpNode* NewPopUpNode = NewObject<UImGUIPopUpNode>(this, PopUpNodesAvailable[i]);
				NewPopUpNode->RegisterComponent();
				RegisterNewNodeToHUD(NewPopUpNode);
				ImNodes::SetNodeScreenSpacePos(NewPopUpNode->NodeID, click_pos);
			}
		}

        ImGui::EndPopup();
    }

	for (auto& Node : CurrentNodesContainedIDMap)
	{
		Node.Value->ExecuteImGUICode();
	}

	// LINKS DISPLAY

	for (auto& Link : LinksCreatedIDMap)
	{
		ImNodes::Link(Link.Key, Link.Value.StartNodeAttributeID, Link.Value.EndNodeAttributeID);
	}

	ImNodes::MiniMap();
	ImNodes::EndNodeEditor();

	// SELECTED ELEMENTS REMOVAL

	if (ImGui::IsKeyReleased(ImGuiKey_Delete))
	{
		int NumSelectedLinks = ImNodes::NumSelectedLinks();
		if (NumSelectedLinks > 0)
		{
			int* SelectedLinks = new int[NumSelectedLinks];
			ImNodes::GetSelectedLinks(SelectedLinks);

			for (int32 i = 0; i < NumSelectedLinks; ++i)
			{
				RemoveLink(SelectedLinks[i]);
			}

			delete[] SelectedLinks;
			SelectedLinks = nullptr;
		}
		int NumSelectedNodes = ImNodes::NumSelectedNodes();
		if (NumSelectedNodes > 0)
		{
			int* SelectedNodes = new int[NumSelectedNodes];
			ImNodes::GetSelectedNodes(SelectedNodes);

			for (int32 i = 0; i < NumSelectedNodes; ++i)
			{
				UImGUINodeBase* NodeToDelete = CurrentNodesContainedIDMap.FindRef(SelectedNodes[i]);
				if (NodeToDelete->GetClass()->IsChildOf(UImGUIPopUpNode::StaticClass()))
				{
					UnregisterNodeFromHUD(NodeToDelete);
					NodeToDelete->DestroyComponent();
				}
			}
			delete[] SelectedNodes;
			SelectedNodes = nullptr;
		}
	}

	// TOGGLE INPUT
	if (ImGui::IsKeyReleased(ImGuiKey_Z))
	{
		GEngine->Exec(GetWorld(), TEXT("ImGui.ToggleInput"));
	}

	// LINKS CREATION

	int IsLinkCreatedStartNodeID;
	int IsLinkCreatedStartAttributeID;
	int IsLinkCreatedEndNodeID;
	int IsLinkCreatedEndAttributeID;
	if (ImNodes::IsLinkCreated(&IsLinkCreatedStartNodeID, &IsLinkCreatedStartAttributeID, &IsLinkCreatedEndNodeID, &IsLinkCreatedEndAttributeID))
	{
		RegisterNewLink(FImGUILink(IsLinkCreatedStartNodeID, IsLinkCreatedStartAttributeID, IsLinkCreatedEndNodeID, IsLinkCreatedEndAttributeID));
	}

	// LINKS DELETION

	int IsLinkDestroyedID;
	if (ImNodes::IsLinkDestroyed(&IsLinkDestroyedID))
	{
		RemoveLink(IsLinkDestroyedID);
	}

	ImGui::End();
}

void AImGUIHUD::RegisterNewLink(const FImGUILink& NewLink)
{
	UImGUINodeBase* BeginNodePtr = CurrentNodesContainedIDMap.FindRef(NewLink.StartNodeID);
	UImGUINodeBase* EndNodePtr = CurrentNodesContainedIDMap.FindRef(NewLink.EndNodeID);
	for (int32 i = 0; i < EndNodePtr->LinksID.Num(); i++)
	{
		if (LinksCreatedIDMap.FindRef(EndNodePtr->LinksID[i]).EndNodeAttributeID == NewLink.EndNodeAttributeID)
		{
			RemoveLink(EndNodePtr->LinksID[i]);
			break;
		}
	}

	const int32 NewLinkID = LinksIDStore.TakeNewID();

	LinksCreatedIDMap.Add(NewLinkID, NewLink);

	BeginNodePtr->LinksID.Add(NewLinkID);
	EndNodePtr->LinksID.Add(NewLinkID);

	static_cast<FImGUIScalarInputAttribute*>(AttributesIDMap.FindRef(NewLink.EndNodeAttributeID))->AttributeValueTiedTo =
		&static_cast<FImGUIScalarOutputAttribute*>(AttributesIDMap.FindRef(NewLink.StartNodeAttributeID))->AttributeValue;
}

void AImGUIHUD::RemoveLink(const int32 LinkID)
{
	FImGUILink RemovedLinkCopy = LinksCreatedIDMap.FindAndRemoveChecked(LinkID);
	static_cast<FImGUIScalarInputAttribute*>(AttributesIDMap.FindRef(RemovedLinkCopy.EndNodeAttributeID))->AttributeValueTiedTo = nullptr;
	CurrentNodesContainedIDMap.FindRef(RemovedLinkCopy.StartNodeID)->LinksID.RemoveSingle(LinkID);
	CurrentNodesContainedIDMap.FindRef(RemovedLinkCopy.EndNodeID)->LinksID.RemoveSingle(LinkID);
	LinksIDStore.ReleaseID(LinkID);
	ImNodes::ClearLinkSelection(LinkID);
}

void AImGUIHUD::BuildSaveStruct(FGraphSave& SaveStruct)
{
	SaveStruct.Clear();
	SaveStruct.Nodes.Reserve(CurrentNodesContainedIDMap.Num());
	for (const auto& Node : CurrentNodesContainedIDMap)
	{
		if (Node.Value->GetClass()->IsChildOf(UImGUIPopUpNode::StaticClass()))
		{
			SaveStruct.Nodes.Add(Node.Value->GetSaveStruct());
		}
		else
		{
			SaveStruct.RegisteredNodes.Add(Node.Value->GetRegisteredSaveStruct());
		}
	}

	LinksCreatedIDMap.GenerateValueArray(SaveStruct.Links);
}

void AImGUIHUD::LoadSaveStruct(const FGraphSave& SaveStruct)
{
	DeleteGraphCompletely();

	for (int32 i = 0; i < SaveStruct.Nodes.Num(); ++i)
	{
		NodesIDStore.RegisterID(SaveStruct.Nodes[i].NodeID);
		for (int32 y = 0; y < SaveStruct.Nodes[i].InputAttributes.Num(); ++y)
		{
			AttributesIDStore.RegisterID(SaveStruct.Nodes[i].InputAttributes[y].AttributeID);
		}
		for (int32 y = 0; y < SaveStruct.Nodes[i].OutputAttributes.Num(); ++y)
		{
			AttributesIDStore.RegisterID(SaveStruct.Nodes[i].OutputAttributes[y].AttributeID);
		}
	}
	for (int32 i = 0; i < SaveStruct.RegisteredNodes.Num(); ++i)
	{
		NodesIDStore.RegisterID(SaveStruct.RegisteredNodes[i].NodeID);
		for (int32 y = 0; y < SaveStruct.RegisteredNodes[i].InputAttributes.Num(); ++y)
		{
			AttributesIDStore.RegisterID(SaveStruct.RegisteredNodes[i].InputAttributes[y].AttributeID);
		}
		for (int32 y = 0; y < SaveStruct.RegisteredNodes[i].OutputAttributes.Num(); ++y)
		{
			AttributesIDStore.RegisterID(SaveStruct.RegisteredNodes[i].OutputAttributes[y].AttributeID);
		}
	}

	TArray<int32> SaveFileInvalidatedAttributesID = TArray<int32>();

	for (int32 i = 0; i < SaveStruct.Nodes.Num(); ++i)
	{
		UImGUINodeBase* NodeObject = NewObject<UImGUINodeBase>(this, SaveStruct.Nodes[i].NodeClass);
		if (NodeObject)
		{
			NodeObject->IsSpawnedFromLoad = true;
			NodeObject->SetupExtraParams(SaveStruct.Nodes[i]);
			NodeObject->RegisterComponent();
			NodeObject->BuildFromSave(SaveStruct.Nodes[i], AttributesIDStore, AttributesIDMap, SaveFileInvalidatedAttributesID);
			CurrentNodesContainedIDMap.Add(NodeObject->NodeID, NodeObject);
		}
	}
	for (int32 i = 0; i < SaveStruct.RegisteredNodes.Num(); ++i)
	{
		TArray<AActor*> RegisteredActors = TArray<AActor*>();
		UGameplayStatics::GetAllActorsWithTag(this, FName("RegisteredActor"), RegisteredActors);
		for (int32 z = 0; z < RegisteredActors.Num(); ++z)
		{
			if (RegisteredActors[z]->Tags.Contains(SaveStruct.RegisteredNodes[i].OwningActorName))
			{
				RegisteredActors[z]->SetActorTransform(SaveStruct.RegisteredNodes[i].OwningActorTransform);
				UImGUINodeBase* NodeObject = NewObject<UImGUINodeBase>(RegisteredActors[z], SaveStruct.RegisteredNodes[i].NodeClass);
				if (NodeObject)
				{
					NodeObject->SetupExtraParams(SaveStruct.RegisteredNodes[i]);
					NodeObject->IsSpawnedFromLoad = true;
					NodeObject->RegisterComponent();
					NodeObject->BuildFromSave(SaveStruct.RegisteredNodes[i], AttributesIDStore, AttributesIDMap, SaveFileInvalidatedAttributesID);
					CurrentNodesContainedIDMap.Add(NodeObject->NodeID, NodeObject);
					RegisteredActors.RemoveAt(z);
				}			
				break;
			}
		}
	}
	
	for (int32 i = 0; i < SaveStruct.Links.Num(); ++i)
	{
		if (!SaveFileInvalidatedAttributesID.Contains(SaveStruct.Links[i].StartNodeAttributeID) && !SaveFileInvalidatedAttributesID.Contains(SaveStruct.Links[i].EndNodeAttributeID))
		{
			RegisterNewLink(SaveStruct.Links[i]);
		}
	}
}

void AImGUIHUD::DeleteGraphCompletely()
{
	for (auto& Node : CurrentNodesContainedIDMap)
	{
		Node.Value->DestroyComponent();
	}

	ImNodes::ClearLinkSelection();
	ImNodes::ClearNodeSelection();
	NodesIDStore.Clear();
	AttributesIDStore.Clear();
	LinksIDStore.Clear();
	CurrentNodesContainedIDMap.Empty();
	LinksCreatedIDMap.Empty();
	AttributesIDMap.Empty();
}