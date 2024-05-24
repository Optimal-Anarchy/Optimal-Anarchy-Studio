// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUIActorScaleNode.h"

UImGUIActorScaleNode::UImGUIActorScaleNode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("New Actor Scale"), 1.f));

	WindowSize = FVector2D(100.f, 50.f);
}

void UImGUIActorScaleNode::ComputeNode()
{
	Super::ComputeNode();
	GetOwner()->SetActorScale3D(FVector(InputAttributes[0].AttributeValue));
}

