// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIRegisteredNode.h"
#include "ImGUIAudioNode.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (ImGUINodes), meta = (BlueprintSpawnableComponent))
class BIOMIPLUGIN_API UImGUIAudioNode : public UImGUIRegisteredNode
{
	GENERATED_BODY()

public:

	UImGUIAudioNode(const FObjectInitializer& ObjectInitializer);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:

	virtual void ComputeNode() override;

	virtual void NodeImGUICode() override;

private:

	void AudioEnvelopBind(const UAudioComponent* AudioComp, const float AverageEnvelopeValue, const float MaxEnvelope, const int32 NumWaveInstances);
	
	TMap<const UAudioComponent*, float> Envelopes;

	// Voice 1
	FManualScalarInputValue Frequency_V1 = FManualScalarInputValue(FName("Frequency_V1"), 40.0f);
	FManualScalarInputValue BinauralBeatFreq_V1 = FManualScalarInputValue(FName("BinauralBeatFreq_V1"), 40.0f);
	FManualScalarInputValue MIX_L_R_V1 = FManualScalarInputValue(FName("MIX_L_R_V1"), 0.548f);
	FManualScalarInputValue Boost_V1 = FManualScalarInputValue(FName("Boost_V1"), 0.0f);
	// Delay
	FManualScalarInputValue Attack_Time_V1 = FManualScalarInputValue(FName("Attack_Time_V1"), 1.0f);
	FManualScalarInputValue Decay_Time_V1 = FManualScalarInputValue(FName("Decay_Time_V1"), 1.0f);
	FManualScalarInputValue Attack_Curve_V1 = FManualScalarInputValue(FName("Attack_Curve_V1"), 1.0f);
	FManualScalarInputValue Decay_Curve_V1 = FManualScalarInputValue(FName("Decay_Curve_V1"), 1.0f);

	// Voice 2
	FManualScalarInputValue Frequency_V2 = FManualScalarInputValue(FName("Frequency_V2"), 10.0f);
	FManualScalarInputValue BinauralBeatFreq_V2 = FManualScalarInputValue(FName("BinauralBeatFreq_V2"), 40.0f);
	FManualScalarInputValue MIX_L_R_V2 = FManualScalarInputValue(FName("MIX_L_R_V2"), 0.548f);
	FManualScalarInputValue Boost_V2 = FManualScalarInputValue(FName("Boost_V2"), 0.0f);
	// Delay
	FManualScalarInputValue Attack_Time_V2 = FManualScalarInputValue(FName("Attack_Time_V2"), 1.0f);
	FManualScalarInputValue Decay_Time_V2 = FManualScalarInputValue(FName("Decay_Time_V2"), 1.0f);
	FManualScalarInputValue Attack_Curve_V2 = FManualScalarInputValue(FName("Attack_Curve_V2"), 1.0f);
	FManualScalarInputValue Decay_Curve_V2 = FManualScalarInputValue(FName("Decay_Curve_V2"), 1.0f);

	// MIX
	FManualScalarInputValue Gain_0 = FManualScalarInputValue(FName("Gain_0"), 1.0f);
	FManualScalarInputValue Gain_1_1 = FManualScalarInputValue(FName("Gain_1_1"), 0.0f);
	
};
