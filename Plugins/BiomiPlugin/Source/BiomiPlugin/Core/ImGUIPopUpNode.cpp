// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUIPopUpNode.h"


void UImGUIPopUpNode::NodeImGUICode()
{
	ImNodes::BeginNodeTitleBar();
	ImGui::SetWindowFontScale(1.2);
	ImGui::TextUnformatted(TCHAR_TO_ANSI(*PopUpGenericName.ToString()));
	ImGui::SetWindowFontScale(1.0);
	ImNodes::EndNodeTitleBar();

	Super::NodeImGUICode();
}