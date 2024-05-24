// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIRegisteredNode.h"
#include "ImGUIActorScaleNode.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (ImGUINodes), meta = (BlueprintSpawnableComponent))
class BIOMIPLUGIN_API UImGUIActorScaleNode : public UImGUIRegisteredNode
{
	GENERATED_BODY()

public:

	UImGUIActorScaleNode(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void ComputeNode() override;
	
};
