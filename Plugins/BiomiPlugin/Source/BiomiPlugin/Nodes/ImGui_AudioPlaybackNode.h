// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomiPlugin/Core/ImGUIPopUpNode.h"
#include "ImGui_AudioPlaybackNode.generated.h"

class USoundBase;
class UAudioComponent;

USTRUCT(BlueprintType)
struct FSoundAssetData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SoundAssetName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* SoundAsset;
};



UCLASS(ClassGroup = ImGUINodes, Blueprintable, BlueprintType)
class BIOMIPLUGIN_API UImGui_AudioPlaybackNode : public UImGUIPopUpNode
{
	GENERATED_BODY()
	
public:
	
	UImGui_AudioPlaybackNode(const FObjectInitializer& ObjectInitializer);



protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual void ComputeNode() override;
	virtual void NodeImGUICode() override;

private:

	// AUDIO AMPLITUDE MULTIPLIER VALUE
	FManualScalarInputValue Volume = FManualScalarInputValue(FName("Volume"), 1.f);

	// AUDIO LOOPING BOOL
	bool ShouldLoop = false;
	
	
	// ARRAY OF ASSETS
	TArray<FSoundAssetData> AudioAssets = TArray<FSoundAssetData>();

	// ITEM SELECTION
	FManualScalarInputValue currentitem = FManualScalarInputValue(FName("CurrentItem"), -1.f);
	int current_item = -1;
	int previous_item = -1;

	// AUDIO PLAYER BINDS
	void AudioEnvelopBind(const UAudioComponent* AudioComp, const float AverageEnvelopeValue, const float MaxEnvelope, const int32 NumWaveInstances);
	void AudioFinishedBind(UAudioComponent* AudioComp);


	// AUDIO ENVELOPE VALUE
	float AudioEnvelopeValue = 0.f;

	

protected:

	UAudioComponent* AudioPlayer = nullptr;;

public:

	UFUNCTION(BlueprintImplementableEvent)
	TArray<FSoundAssetData> GetSoundAssets();


};
