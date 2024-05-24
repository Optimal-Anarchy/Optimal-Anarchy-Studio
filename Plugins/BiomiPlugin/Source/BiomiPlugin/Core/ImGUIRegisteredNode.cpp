// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUIRegisteredNode.h"
#include "ImGUIHUD.h"

void UImGUIRegisteredNode::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsSpawnedFromLoad)
	{
		GetOwner()->Tags.Add(FName("RegisteredActor"));
		GetOwner()->Tags.Add(FName(GetOwner()->GetName()));
		AImGUIHUD::RegisterNewNodeToHUD(this);
	}
}

void UImGUIRegisteredNode::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	AImGUIHUD::UnregisterNodeFromHUD(this);

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UImGUIRegisteredNode::NodeImGUICode()
{
	ImNodes::BeginNodeTitleBar(); 
	ImGui::SetWindowFontScale(1.2);  
	ImGui::TextUnformatted(TCHAR_TO_ANSI(*GetOwner()->GetName()));
	ImGui::SetWindowFontScale(1.0);
	ImNodes::EndNodeTitleBar();

	Super::NodeImGUICode();
}

