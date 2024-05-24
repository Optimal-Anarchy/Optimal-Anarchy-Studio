// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGuiMainWidget.h"
#include "SaveManager.h"

void UImGuiMainWidget::NativeConstruct()
{
	SaveManager = NewObject<USaveManager>(this);
	Super::NativeConstruct();
}