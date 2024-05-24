// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplyNode.h"

UMultiplyNode::UMultiplyNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PopUpGenericName = FText::FromString("Multiply A*B ");

	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("A"),0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("B"),0.f));

	OutputAttributes.Add(FImGUIScalarOutputAttribute(FText::FromString("Result")));
}

void UMultiplyNode::ComputeNode()
{
	OutputAttributes[0].AttributeValue = InputAttributes[0].AttributeValue * InputAttributes[1].AttributeValue;
}

void UMultiplyNode::NodeImGUICode()
{
	Super::NodeImGUICode();
}
