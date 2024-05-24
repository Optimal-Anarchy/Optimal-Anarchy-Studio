// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BIOMIPLUGIN_API IActorInterface
{
	GENERATED_BODY()

	// Actor Interface Functions
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Actor Interface|Getters")
	bool IsActorMoving();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Actor Interface|Setters")
	void SetActorMoving(bool bIsMoving);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Actor Interface|Material")
	void SetEmmisiveIntensity(float NewIntensity);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Actor Interface|Material")
	void SetMaterialColor(FVector NewColor);

	

	IActorInterface() = default;
};
