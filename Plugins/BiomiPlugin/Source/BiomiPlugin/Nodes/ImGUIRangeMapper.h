// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIPopUpNode.h"
#include "ImGUIRangeMapper.generated.h"

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API UImGUIRangeMapper : public UImGUIPopUpNode
{
	GENERATED_BODY()

	UImGUIRangeMapper(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void ComputeNode() override;

	virtual void NodeImGUICode() override;

private:

	FManualScalarInputValue InputMin = FManualScalarInputValue(FName("InputMin"));

	FManualScalarInputValue InputMax = FManualScalarInputValue(FName("InputMax"));

	FManualScalarInputValue OutputMin = FManualScalarInputValue(FName("OutputMin"));

	FManualScalarInputValue OutputMax = FManualScalarInputValue(FName("OutputMax"));

};
