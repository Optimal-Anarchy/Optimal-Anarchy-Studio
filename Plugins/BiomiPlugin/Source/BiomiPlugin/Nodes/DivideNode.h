// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIPopUpNode.h"
#include "DivideNode.generated.h"

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API UDivideNode : public UImGUIPopUpNode
{
	GENERATED_BODY()
	
	UDivideNode(const FObjectInitializer& ObjectInitializer); 

protected:
	virtual void ComputeNode() override;
};
