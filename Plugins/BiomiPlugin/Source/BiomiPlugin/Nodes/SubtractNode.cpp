// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtractNode.h"

USubtractNode::USubtractNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PopUpGenericName = FText::FromString("Subtract A-B ");

	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("A"), 0));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("B"), 0));

	OutputAttributes.Add(FImGUIScalarOutputAttribute(FText::FromString("Result")));

}


void USubtractNode::ComputeNode()
{
	OutputAttributes[0].AttributeValue = InputAttributes[0].AttributeValue - InputAttributes[1].AttributeValue;
}
