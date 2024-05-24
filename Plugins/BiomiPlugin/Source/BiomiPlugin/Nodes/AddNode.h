// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIPopUpNode.h"
#include "AddNode.generated.h"

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API UAddNode : public UImGUIPopUpNode
{
	GENERATED_BODY()

	UAddNode(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void ComputeNode() override;
};
