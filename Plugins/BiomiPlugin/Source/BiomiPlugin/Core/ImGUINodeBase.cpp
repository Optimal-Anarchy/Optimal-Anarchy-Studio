// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUINodeBase.h"
#include "Serialization/ArrayWriter.h"
#include "Serialization/ArrayReader.h"


UImGUINodeBase::UImGUINodeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	if (ClassGUID.IsValid())
	{
		FGuid CurrentGUID = ClassGUID;
		UClass* ParentClass = GetClass()->GetSuperClass();
		LoadConfig(ParentClass, 0, 0U, GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UImGUINodeBase, ClassGUID)));
		if (ClassGUID == CurrentGUID)
		{
			ClassGUID = FGuid::NewGuid();
		}
	}
	else
	{
		ClassGUID = FGuid::NewGuid();
	}	
}

void UImGUINodeBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UpdateInputValues())
	{
		ComputeNode();
	}
}

void UImGUINodeBase::BeginPlay()
{
	Super::BeginPlay();

	ComputeNode();
}

void UImGUINodeBase::InitiateNode(const int32 InNodeID, const TArray<int32>& AttributeIDsProvided)
{
	NodeID = InNodeID;
	
	for (int32 i = 0; i < OutputAttributes.Num(); ++i)
	{
		OutputAttributes[i].AttributeID = AttributeIDsProvided[i];
	}
	for (int32 i = 0; i < InputAttributes.Num(); ++i)
	{
		InputAttributes[i].AttributeID = AttributeIDsProvided[i + OutputAttributes.Num()];
	}
}

int32 UImGUINodeBase::GetTotalNumberOfAttributes() const
{
	return OutputAttributes.Num() + InputAttributes.Num();
}

void UImGUINodeBase::ExecuteImGUICode()
{
	ImNodes::PushColorStyle(ImNodesCol_NodeOutline, ImNodes::IsNodeSelected(NodeID) || ImNodes::IsNodeHovered(&NodeID) ? IM_COL32(0, 255, 255, 255) : IM_COL32(50, 50, 50, 255));
	ImNodes::BeginNode(NodeID);
	NodeImGUICode();

	for (int32 i = 0; i < FMath::Max(OutputAttributes.Num(), InputAttributes.Num()); ++i)
	{
		if (InputAttributes.IsValidIndex(i))
		{
			ImNodes::BeginInputAttribute(InputAttributes[i].AttributeID, 2);
			ImGui::Text(TCHAR_TO_ANSI(*InputAttributes[i].AttributeName.ToString()));
			ImNodes::EndInputAttribute();
		}
		if (OutputAttributes.IsValidIndex(i))
		{
			if (InputAttributes.IsValidIndex(i))
			{
				ImGui::SameLine(WindowSize.X - ImGui::CalcTextSize(TCHAR_TO_ANSI(*OutputAttributes[i].AttributeName.ToString())).x);
			}
			else
			{
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + WindowSize.X - ImGui::CalcTextSize(TCHAR_TO_ANSI(*OutputAttributes[i].AttributeName.ToString())).x);
			}
			ImNodes::BeginOutputAttribute(OutputAttributes[i].AttributeID, 2);
			ImGui::Text(TCHAR_TO_ANSI(*OutputAttributes[i].AttributeName.ToString()));
			ImNodes::EndOutputAttribute();
		}
	}
	ImNodes::PopColorStyle();
	ImNodes::EndNode();
}

void UImGUINodeBase::NodeImGUICode()
{
	
}

bool UImGUINodeBase::IsInputLinked(int i)
{
	if (InputAttributes.IsValidIndex(i))
	{
		return InputAttributes[i].IsLinked();
	}
	else
	{
		return false;
	}
}

void UImGUINodeBase::ComputeNode()
{

}

bool UImGUINodeBase::UpdateInputValues()
{
	bool AnyValueUpdated = false;
	for (int32 i = 0; i < InputAttributes.Num(); ++i)
	{
		if (InputAttributes[i].UpdateValue())
		{
			AnyValueUpdated = true;
		}
	}

	for (int32 i = 0; i < ManualInputValues.Num(); ++i)
	{
		if (ManualInputValues[i]->UpdateValue())
		{
			AnyValueUpdated = true;
		}
	}

	return AnyValueUpdated;
}

FNodeSave UImGUINodeBase::GetSaveStruct() 
{
	return FNodeSave(ClassGUID, GetClass(), NodeID, ManualInputValues, InputAttributes, OutputAttributes, ContructExtraPropertySave());
}

FRegisteredNodeSave UImGUINodeBase::GetRegisteredSaveStruct()
{
	return FRegisteredNodeSave(ClassGUID, GetClass(), NodeID, ManualInputValues, InputAttributes, OutputAttributes, ContructExtraPropertySave(), GetOwner()->GetName(), GetOwner()->GetActorTransform());
}

TArray<FExtraPropertySave> UImGUINodeBase::ContructExtraPropertySave()
{
	TArray<FExtraPropertySave> ExtraPropertySave = TArray<FExtraPropertySave>();
	if (ExtraParamsToSave.Num() > 0)
	{
		ExtraPropertySave.SetNum(ExtraParamsToSave.Num());
		for (int32 i = 0; i < ExtraParamsToSave.Num(); ++i)
		{
			ExtraPropertySave[i].PropertyName = FName(ExtraParamsToSave[i]->GetName());
			ExtraParamsToSave[i]->ExportText_Direct(ExtraPropertySave[i].PropertyValue, ExtraParamsToSave[i]->ContainerPtrToValuePtr<void*>(this),
				ExtraParamsToSave[i]->ContainerPtrToValuePtr<void*>(this), this, 0);
		}
	}
	return ExtraPropertySave;
}

void UImGUINodeBase::SetupExtraParams(const FNodeSave& NodeSave)
{
	for (int32 i = 0; i < ExtraParamsToSave.Num(); ++i)
	{
		for (int32 y = 0; y < NodeSave.ExtraPropertySave.Num(); ++y)
		{
			if (ExtraParamsToSave[i]->GetName() == NodeSave.ExtraPropertySave[y].PropertyName)
			{
				ExtraParamsToSave[i]->ImportText_Direct(*NodeSave.ExtraPropertySave[y].PropertyValue, ExtraParamsToSave[i]->ContainerPtrToValuePtr<void*>(this), this, 0);
				break;
			}
		}
	}
}

void UImGUINodeBase::BuildFromSave(const FNodeSave& NodeSave, FIDStore& AttributeIDStore, TMap<int32, FImGUIScalarAttribute*>& AttributeIDMap, TArray<int32>& InvalidatedID)
{
	NodeID = NodeSave.NodeID;
	ImVec2 NewGridPos;
	NewGridPos.x = NodeSave.NodeScreenSpace.X;
	NewGridPos.y = NodeSave.NodeScreenSpace.Y;
	ImNodes::SetNodeGridSpacePos(NodeID, NewGridPos);

	for (int32 i = 0; i < ManualInputValues.Num(); ++i)
	{
		for (int32 y = 0; y < NodeSave.ManualInputValues.Num(); ++y)
		{
			if (ManualInputValues[i]->ManualValueName == NodeSave.ManualInputValues[y].InputName)
			{
				ManualInputValues[i]->CurrentValue = NodeSave.ManualInputValues[y].InputValue;
				break;
			}
		}
	}
	for (int32 i = 0; i < InputAttributes.Num(); ++i)
	{
		bool SaveFound = false;
		for (int32 y = 0; y < NodeSave.InputAttributes.Num(); ++y)
		{
			if (InputAttributes[i].AttributeName.ToString() == NodeSave.InputAttributes[y].AttributeName.ToString())
			{
				InputAttributes[i].AttributeID = NodeSave.InputAttributes[y].AttributeID;
				AttributeIDMap.Add(InputAttributes[i].AttributeID, &InputAttributes[i]);
				SaveFound = true;
				break;
			}
		}
		if (!SaveFound)
		{
			InputAttributes[i].AttributeID = AttributeIDStore.TakeNewID();
			AttributeIDMap.Add(InputAttributes[i].AttributeID, &InputAttributes[i]);
		}
	}
	for (int32 i = 0; i < OutputAttributes.Num(); ++i)
	{
		bool SaveFound = false;
		for (int32 y = 0; y < NodeSave.OutputAttributes.Num(); ++y)
		{
			if (OutputAttributes[i].AttributeName.ToString() == NodeSave.OutputAttributes[y].AttributeName.ToString())
			{
				OutputAttributes[i].AttributeID = NodeSave.OutputAttributes[y].AttributeID;
				AttributeIDMap.Add(OutputAttributes[i].AttributeID, &OutputAttributes[i]);
				SaveFound = true;
				break;
			}
		}
		if (!SaveFound)
		{
			OutputAttributes[i].AttributeID = AttributeIDStore.TakeNewID();
			AttributeIDMap.Add(OutputAttributes[i].AttributeID, &OutputAttributes[i]);
		}
	}

	for (int32 i = 0; i < NodeSave.InputAttributes.Num(); ++i)
	{
		for (int32 y = 0; y < InputAttributes.Num(); ++y)
		{
			if (NodeSave.InputAttributes[i].AttributeName.ToString() == InputAttributes[y].AttributeName.ToString())
			{
				break;
			}
			else if (y == InputAttributes.Num() - 1)
			{
				InvalidatedID.Add(NodeSave.InputAttributes[i].AttributeID);
			}
		}

	}
	for (int32 i = 0; i < NodeSave.OutputAttributes.Num(); ++i)
	{
		for (int32 y = 0; y < OutputAttributes.Num(); ++y)
		{
			if (NodeSave.OutputAttributes[i].AttributeName.ToString() == OutputAttributes[y].AttributeName.ToString())
			{
				break;
			}
			else if (y == OutputAttributes.Num() - 1)
			{
				InvalidatedID.Add(NodeSave.OutputAttributes[i].AttributeID);
			}
		}
	}
}