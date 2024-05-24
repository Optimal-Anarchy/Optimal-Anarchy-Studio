// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUIBasicScalar.h"

UImGUIBasicScalar::UImGUIBasicScalar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PopUpGenericName = FText::FromString("Scalar");

	OutputAttributes.Add(FImGUIScalarOutputAttribute(FText::FromString("Out")));

	ManualInputValues.Add(&ScalarValue);
}

void UImGUIBasicScalar::ComputeNode()
{
	Super::ComputeNode();
	OutputAttributes[0].AttributeValue = ScalarValue.CurrentValue;
}

void UImGUIBasicScalar::NodeImGUICode()
{
	Super::NodeImGUICode();
	ImGui::PushItemWidth(50.f);
	ImGui::InputFloat("Value", &ScalarValue.CurrentValue);
	ImGui::PopItemWidth();
}