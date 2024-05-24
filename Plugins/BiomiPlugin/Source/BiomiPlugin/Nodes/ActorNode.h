// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIPopUpNode.h"
#include "ActorNode.generated.h"

/**
 * FORWARD DECLARATIONS
 */

class AActor;

UCLASS(ClassGroup = ImGUINodes, Blueprintable, BlueprintType)
class BIOMIPLUGIN_API UActorNode : public UImGUIPopUpNode
{
	GENERATED_BODY()
	
	UActorNode(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;


	// NODE FUNCTIONS
	virtual void ComputeNode() override;
	virtual void NodeImGUICode() override;
	virtual void ExecuteImGUICode() override;

	// INPUT ATTRIBUTE
	void AddInputAttribute(int i, FString CustomName = FString());

	// ADD DRAG FLOAT
	bool AddDragFloat(const char* label, float* value);

protected:


	//Color RGB
	FManualScalarInputValue Color_R = FManualScalarInputValue(FName("Color_R"));
	FManualScalarInputValue Color_G = FManualScalarInputValue(FName("Color_G"));
	FManualScalarInputValue Color_B = FManualScalarInputValue(FName("Color_B"));

	//Translation XYZ
	FManualScalarInputValue Translation_X = FManualScalarInputValue(FName("Translation_X"));
	FManualScalarInputValue Translation_Y = FManualScalarInputValue(FName("Translation_Y"));
	FManualScalarInputValue Translation_Z = FManualScalarInputValue(FName("Translation_Z"));

	//Rotation XYZ
	FManualScalarInputValue Rotation_X = FManualScalarInputValue(FName("Rotation_X"));
	FManualScalarInputValue Rotation_Y = FManualScalarInputValue(FName("Rotation_Y"));
	FManualScalarInputValue Rotation_Z = FManualScalarInputValue(FName("Rotation_Z"));

	//Scale XYZ
	FManualScalarInputValue Scale_X = FManualScalarInputValue(FName("Scale_X"),1.f);
	FManualScalarInputValue Scale_Y = FManualScalarInputValue(FName("Scale_Y"),1.f);
	FManualScalarInputValue Scale_Z = FManualScalarInputValue(FName("Scale_Z"),1.f);

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Actor Node|Settings")
	TSubclassOf<AActor> ActorClass;


private :
	
	AActor* ActorReference = nullptr;
};
