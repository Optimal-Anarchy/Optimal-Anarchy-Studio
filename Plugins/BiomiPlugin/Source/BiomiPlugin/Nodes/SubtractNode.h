// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIPopUpNode.h"
#include "SubtractNode.generated.h"

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API USubtractNode : public UImGUIPopUpNode
{
	GENERATED_BODY()
	
	USubtractNode(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void ComputeNode() override;
};
