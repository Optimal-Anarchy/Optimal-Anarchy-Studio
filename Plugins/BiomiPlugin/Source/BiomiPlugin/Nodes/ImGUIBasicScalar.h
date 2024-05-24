// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIPopUpNode.h"
#include "ImGUIBasicScalar.generated.h"

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API UImGUIBasicScalar : public UImGUIPopUpNode
{
	GENERATED_BODY()

public:

	UImGUIBasicScalar(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void ComputeNode() override;

	virtual void NodeImGUICode() override;

private:

	FManualScalarInputValue ScalarValue = FManualScalarInputValue(FName("ScalarValue"), 1.f);
	
};
