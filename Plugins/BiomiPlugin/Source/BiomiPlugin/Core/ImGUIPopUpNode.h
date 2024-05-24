// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGUINodeBase.h"
#include "ImGUIPopUpNode.generated.h"

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API UImGUIPopUpNode : public UImGUINodeBase
{
	GENERATED_BODY()

public:

	FText PopUpGenericName = FText::FromString("Popup");

protected:

	virtual void NodeImGUICode() override;
	
};
