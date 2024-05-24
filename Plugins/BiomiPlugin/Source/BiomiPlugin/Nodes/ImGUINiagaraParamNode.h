// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIRegisteredNode.h"
#include "ImGUINiagaraParamNode.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (ImGUINodes), meta = (BlueprintSpawnableComponent))
class BIOMIPLUGIN_API UImGUINiagaraParamNode : public UImGUIRegisteredNode
{
	GENERATED_BODY()
	
public:

	UImGUINiagaraParamNode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FName ParamToControl = NAME_None;

protected:

	virtual void ComputeNode() override;

};
