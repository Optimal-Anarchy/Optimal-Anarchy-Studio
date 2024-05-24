// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUINiagaraParamNode.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"

UImGUINiagaraParamNode::UImGUINiagaraParamNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("In"), 0.f));

	ExtraParamsToSave.Add(GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UImGUINiagaraParamNode, ParamToControl)));
}

void UImGUINiagaraParamNode::BeginPlay()
{
	if (!ParamToControl.IsNone())
	{
		InputAttributes[0].AttributeName = FText::FromName(ParamToControl);
	}
	Super::BeginPlay();
}

void UImGUINiagaraParamNode::ComputeNode()
{
	Super::ComputeNode();
	if (ANiagaraActor* OwningNiagaraActor = Cast<ANiagaraActor>(GetOwner()))
	{
		OwningNiagaraActor->GetNiagaraComponent()->SetFloatParameter(ParamToControl, InputAttributes[0].AttributeValue);
	}
}

