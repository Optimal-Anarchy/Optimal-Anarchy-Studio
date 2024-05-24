// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ImGuiMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class BIOMIPLUGIN_API UImGuiMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(BlueprintReadOnly)
	class USaveManager* SaveManager;

};
