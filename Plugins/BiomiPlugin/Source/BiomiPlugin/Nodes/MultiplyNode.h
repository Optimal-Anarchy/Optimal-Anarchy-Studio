// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIPopUpNode.h"
#include "MultiplyNode.generated.h"

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API UMultiplyNode : public UImGUIPopUpNode
{
	GENERATED_BODY()

	UMultiplyNode(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void ComputeNode() override;

	virtual void NodeImGUICode() override;
};
