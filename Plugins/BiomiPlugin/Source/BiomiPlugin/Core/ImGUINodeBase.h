// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <imgui.h> 
#include <imnodes.h>
#include "MiscLibrary.h"
#include "ImGUINodeBase.generated.h"


USTRUCT()
struct FImGUIScalarAttribute
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FText AttributeName = FText::FromString("Name");

	UPROPERTY()
	int32 AttributeID = -1;

	FImGUIScalarAttribute() {}

	FImGUIScalarAttribute(const FText InAttributeName)
	{
		AttributeName = InAttributeName;
	}

};

USTRUCT()
struct FImGUIScalarOutputAttribute : public FImGUIScalarAttribute
{
	GENERATED_USTRUCT_BODY()

	float AttributeValue = 0.f;

	FImGUIScalarOutputAttribute() {}

	FImGUIScalarOutputAttribute(const FText InAttributeName)
	{
		AttributeName = InAttributeName;
	}

};

USTRUCT()
struct FImGUIScalarInputAttribute : public FImGUIScalarOutputAttribute
{
	GENERATED_USTRUCT_BODY()

	float* AttributeValueTiedTo = nullptr;

	float AttributeDefaultValue = 0.f;

	FImGUIScalarInputAttribute() {}

	FImGUIScalarInputAttribute(const FText InAttributeName, const float InAttributeDefaultValue = 0.f)
	{
		AttributeName = InAttributeName;
		AttributeDefaultValue = InAttributeDefaultValue;
		AttributeValue = InAttributeDefaultValue;
	}

	/** returns true if the value was updated */
	bool UpdateValue()
	{
		float ValueToCompare = (AttributeValueTiedTo ? *AttributeValueTiedTo : AttributeDefaultValue);
		if (AttributeValue != ValueToCompare)
		{
			AttributeValue = ValueToCompare;
			return true;
		}
		return false;
	}

	bool IsLinked() const
	{
		if (AttributeValueTiedTo)
		{
			return true;
		}
		return false;
	}
};

USTRUCT()
struct FManualScalarInputValue
{
	GENERATED_USTRUCT_BODY()

	FName ManualValueName = NAME_None;

	float CurrentValue = 0.f;

	float PreviousValue = 0.f;

	FManualScalarInputValue() {}

	FManualScalarInputValue(const FName& InManualValueName)
	{
		ManualValueName = InManualValueName;
	}

	FManualScalarInputValue(const FName& InManualValueName, const float InDefaultValue)
	{
		ManualValueName = InManualValueName;
		CurrentValue = InDefaultValue;
		PreviousValue = InDefaultValue;
	}

	bool UpdateValue()
	{
		if (CurrentValue != PreviousValue)
		{
			PreviousValue = CurrentValue;
			return true;
		}
		return false;
	}
};

USTRUCT()
struct FManualInputSave
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FName InputName = NAME_None;

	UPROPERTY()
	float InputValue = 0.f;

	FManualInputSave() {}

	FManualInputSave(const FManualScalarInputValue& ManualScalar)
	{
		InputName = ManualScalar.ManualValueName;
		InputValue = ManualScalar.CurrentValue;
	}
};

USTRUCT()
struct FExtraPropertySave
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FName PropertyName = NAME_None;

	UPROPERTY()
	FString PropertyValue = FString();

	FExtraPropertySave() {}
};

USTRUCT()
struct FNodeSave
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FGuid NodeGUID = FGuid();

	UPROPERTY()
	TSubclassOf<class UImGUINodeBase> NodeClass = UObject::StaticClass();

	UPROPERTY()
	int32 NodeID = -1;

	UPROPERTY()
	FVector2D NodeScreenSpace = FVector2D::ZeroVector;

	UPROPERTY()
	TArray<FManualInputSave> ManualInputValues = TArray<FManualInputSave>();

	UPROPERTY()
	TArray<FImGUIScalarAttribute> InputAttributes = TArray<FImGUIScalarAttribute>();

	UPROPERTY()
	TArray<FImGUIScalarAttribute> OutputAttributes = TArray<FImGUIScalarAttribute>();

	UPROPERTY()
	TArray<FExtraPropertySave> ExtraPropertySave = TArray<FExtraPropertySave>();

	FNodeSave() {}

	FNodeSave(const FGuid& InNodeGUID, const TSubclassOf<UImGUINodeBase> InNodeClass, const int32 InNodeID, const TArray<FManualScalarInputValue*>& InManualInputValues,
		const TArray<FImGUIScalarInputAttribute>& InInputAttributes, const TArray<FImGUIScalarOutputAttribute>& InOutputAttributes, const TArray<FExtraPropertySave>& InExtraPropertySave)
	{
		SetupValues(InNodeGUID, InNodeClass, InNodeID, InManualInputValues, InInputAttributes, InOutputAttributes, InExtraPropertySave);
	}

	void SetupValues(const FGuid& InNodeGUID, const TSubclassOf<UImGUINodeBase> InNodeClass, const int32 InNodeID, const TArray<FManualScalarInputValue*>& InManualInputValues,
		const TArray<FImGUIScalarInputAttribute>& InInputAttributes, const TArray<FImGUIScalarOutputAttribute>& InOutputAttributes, const TArray<FExtraPropertySave>& InExtraPropertySave)
	{
		NodeGUID = InNodeGUID;
		NodeClass = InNodeClass;
		NodeID = InNodeID;
		ImVec2 ScreenPos = ImNodes::GetNodeGridSpacePos(NodeID);
		NodeScreenSpace = FVector2D(ScreenPos.x, ScreenPos.y);
		ExtraPropertySave = InExtraPropertySave;

		ManualInputValues.SetNum(InManualInputValues.Num());
		for (int32 i = 0; i < InManualInputValues.Num(); ++i)
		{
			ManualInputValues[i].InputName = InManualInputValues[i]->ManualValueName;
			ManualInputValues[i].InputValue = InManualInputValues[i]->CurrentValue;
		}

		InputAttributes.SetNum(InInputAttributes.Num());
		for (int32 i = 0; i < InInputAttributes.Num(); ++i)
		{
			InputAttributes[i] = InInputAttributes[i];
		}

		OutputAttributes.SetNum(InOutputAttributes.Num());
		for (int32 i = 0; i < InOutputAttributes.Num(); ++i)
		{
			OutputAttributes[i] = InOutputAttributes[i];
		}
	}
};

USTRUCT()
struct FRegisteredNodeSave : public FNodeSave
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString OwningActorName = FString();

	UPROPERTY()
	FTransform OwningActorTransform = FTransform();

	FRegisteredNodeSave() {}

	FRegisteredNodeSave(const FGuid& InNodeGUID, const TSubclassOf<UImGUINodeBase> InNodeClass, const int32 InNodeID, const TArray<FManualScalarInputValue*>& InManualInputValues,
		const TArray<FImGUIScalarInputAttribute>& InInputAttributes,const TArray<FImGUIScalarOutputAttribute>& InOutputAttributes, 
		const TArray<FExtraPropertySave>& InExtraPropertySave, const FString& InOwningActorName, const FTransform& InOwningActorTransform)
	{
		SetupValues(InNodeGUID, InNodeClass, InNodeID, InManualInputValues, InInputAttributes, InOutputAttributes, InExtraPropertySave);

		OwningActorName = InOwningActorName;
		OwningActorTransform = InOwningActorTransform;
	}
};

UCLASS(Config=Game)
class BIOMIPLUGIN_API UImGUINodeBase : public UActorComponent
{
	GENERATED_BODY()

public:	

	UImGUINodeBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitiateNode(const int32 InNodeID, const TArray<int32>& AttributeIDsProvided);

	int32 GetTotalNumberOfAttributes() const;

	virtual void ExecuteImGUICode();

	TArray<FImGUIScalarInputAttribute> InputAttributes;

	TArray<FImGUIScalarOutputAttribute> OutputAttributes;
	
	int32 NodeID = -1;

	TArray<int32> LinksID;

	FVector2D WindowSize = FVector2D(80.f, 45.f);

	FNodeSave GetSaveStruct();

	FRegisteredNodeSave GetRegisteredSaveStruct();

	void SetupExtraParams(const FNodeSave& NodeSave);

	void BuildFromSave(const FNodeSave& NodeSave, FIDStore& AttributeIDStore, TMap<int32, FImGUIScalarAttribute*>& AttributeIDMap, TArray<int32>& InvalidatedID);

	bool IsSpawnedFromLoad = false;

protected:

	UPROPERTY(Config)
	FGuid ClassGUID = FGuid();

	virtual void ComputeNode();

	virtual void NodeImGUICode();

	bool IsInputLinked(int i);

	TArray<FManualScalarInputValue*> ManualInputValues;

	TArray<FProperty*> ExtraParamsToSave;

private:

	/** returns true if any value was updated */
	bool UpdateInputValues();

	TArray<FExtraPropertySave> ContructExtraPropertySave();
		
};
