// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUIRangeMapper.h"


UImGUIRangeMapper::UImGUIRangeMapper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PopUpGenericName = FText::FromString("Range Mapper");

	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("In")));
	OutputAttributes.Add(FImGUIScalarOutputAttribute(FText::FromString("Out")));

	ManualInputValues.SetNum(4);
	ManualInputValues[0] = &InputMin;
	ManualInputValues[1] = &InputMax;
	ManualInputValues[2] = &OutputMin;
	ManualInputValues[3] = &OutputMax;

	WindowSize = FVector2D(250.f, 60.f);
}

void UImGUIRangeMapper::ComputeNode()
{
	Super::ComputeNode();
	OutputAttributes[0].AttributeValue = 
		FMath::GetMappedRangeValueClamped(FVector2D(InputMin.CurrentValue, InputMax.CurrentValue), 
			FVector2D(OutputMin.CurrentValue, OutputMax.CurrentValue), InputAttributes[0].AttributeValue);
}

void UImGUIRangeMapper::NodeImGUICode()
{
	Super::NodeImGUICode();

	ImGui::TextUnformatted("Input Range");
	ImGui::SameLine(WindowSize.X - ImGui::CalcTextSize("Output Range").x);
	ImGui::TextUnformatted("Output Range");
	ImGui::PushItemWidth(50.f);
	ImGui::InputFloat("In Min", &InputMin.CurrentValue);
	ImGui::SameLine(WindowSize.X - (ImGui::CalcItemWidth() + ImGui::CalcTextSize("Out Min").x));
	ImGui::InputFloat("Out Min", &OutputMin.CurrentValue);
	ImGui::InputFloat("In Max", &InputMax.CurrentValue);
	ImGui::SameLine(WindowSize.X - (ImGui::CalcItemWidth() + ImGui::CalcTextSize("Out Max").x));
	ImGui::InputFloat("Out Max", &OutputMax.CurrentValue);
	ImGui::PopItemWidth();
}