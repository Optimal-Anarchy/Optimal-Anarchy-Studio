// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGUINodeBase.h"
#include "ImGUIRegisteredNode.generated.h"

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API UImGUIRegisteredNode : public UImGUINodeBase
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

protected:

	virtual void NodeImGUICode() override;

};
