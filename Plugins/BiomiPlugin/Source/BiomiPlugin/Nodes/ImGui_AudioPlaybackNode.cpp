// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGui_AudioPlaybackNode.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"




UImGui_AudioPlaybackNode::UImGui_AudioPlaybackNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// DISPLAY NAME FOR THE NODE
	PopUpGenericName = FText::FromString("Audio Playback");

	// OUTPUT ATTRIBUTES
	OutputAttributes.Add(FText::FromString("Out"));

	//MANUAL VALUES
	ManualInputValues.Add(&Volume);
	ManualInputValues.Add(&currentitem);

	// NODE SIZE
	WindowSize = FVector2D(150, 80);
}

void UImGui_AudioPlaybackNode::BeginPlay()
{
	Super::BeginPlay();

	// CREATE A NEW AUDIO COMPONENT and ADD IT TO OWNER CLASS
	AudioPlayer = NewObject<UAudioComponent>(GetOwner());
	AudioPlayer->RegisterComponent();

	// BIND AUDIO ENVELOPE FUNCTION
	if (AudioPlayer)
	{
		AudioPlayer->OnAudioMultiEnvelopeValueNative.AddUObject(this, &UImGui_AudioPlaybackNode::AudioEnvelopBind);
		AudioPlayer->OnAudioFinishedNative.AddUObject(this, &UImGui_AudioPlaybackNode::AudioFinishedBind);
	}


	// FILL AUDIO ASSETS ARRAY USING BLUEPRINT FUNCTION
	AudioAssets = GetSoundAssets();


}

void UImGui_AudioPlaybackNode::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);

	// CHECK IF SOUND HAS CHANGED
	if (previous_item != FMath::FloorToInt(currentitem.CurrentValue))
	{
		if (AudioPlayer)
		{
			// STOP AUDIO PLAYER AND SET NEW AUDIO ASSET
			AudioPlayer->Stop();
			AudioPlayer->SetSound(AudioAssets[FMath::FloorToInt(currentitem.CurrentValue)].SoundAsset);
			AudioPlayer->Play();
			previous_item = FMath::FloorToInt(currentitem.CurrentValue);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, "Audio Changed");
		}
	}

	// SET VOLUME MULTIPLIER
	if (AudioPlayer)
	{
		AudioPlayer->SetVolumeMultiplier(Volume.CurrentValue);
	}


	// SET OUTPUT ATTRIBUTE VALUE 
	OutputAttributes[0].AttributeValue = AudioEnvelopeValue;

}

void UImGui_AudioPlaybackNode::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (AudioPlayer)
	{
		AudioPlayer->DestroyComponent();
	}
}

void UImGui_AudioPlaybackNode::ComputeNode()
{
	Super::ComputeNode();

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, "Computing Node");

}

void UImGui_AudioPlaybackNode::NodeImGUICode()
{
	Super::NodeImGUICode();

	ImGui::PushItemWidth(100.f);


	// DROP DOWN FOR AUDIO SELECTION
	if(ImGui::BeginCombo("Audio File", FMath::FloorToInt(currentitem.CurrentValue) > -1 ? TCHAR_TO_ANSI(*AudioAssets[FMath::FloorToInt(currentitem.CurrentValue)].SoundAssetName) : "Choose..."))
	{
		for (int i = 0; i < AudioAssets.Num(); i++ )
		{
			const char* AudioNameInChar = TCHAR_TO_ANSI(*AudioAssets[i].SoundAssetName);
			bool bIsSelected = (FMath::FloorToInt(currentitem.CurrentValue) == i);

			if(ImGui::Selectable(AudioNameInChar,bIsSelected))
			{
				currentitem.CurrentValue = float(i);
			}

			if(bIsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	// SLIDER FOR VOLUME
	ImGui::DragFloat("Volume", &Volume.CurrentValue, 0.1f, 0, 100, "%.3f", ImGuiSliderFlags_AlwaysClamp);

	ImGui::Separator();


	// LOOPING CHECKBOX
	if (ImGui::Checkbox("Loop", &ShouldLoop))
	{
		// DO NOTHING HERE
	}


	// PLAY/PAUSE BUTTON
	if (AudioPlayer)
	{
		if (ImGui::Button( (AudioPlayer->IsPlaying() && !AudioPlayer->bIsPaused) ? "Pause" : "Play", ImVec2(50, 50)))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Testing");
			if (!AudioPlayer->IsPlaying())
			{
				AudioPlayer->Play();
				AudioPlayer->SetPaused(false);
			}
			AudioPlayer->SetPaused(!AudioPlayer->bIsPaused);
			
		}
	}

	ImGui::SameLine(WindowSize.X - (50));
	
	// PLAY ALL BUTTON
	if (ImGui::Button("PLAY ALL", ImVec2(80,50)))
	{
		if (GetOwner())
		{
			TArray<UAudioComponent*> AudioComps;
			GetOwner()->GetComponents<UAudioComponent>(AudioComps);

			if (AudioComps.Num() > 0)
			{
				for (UAudioComponent* AudioComp : AudioComps)
				{
					AudioComp->Play();
					AudioComp->SetPaused(false);
				}
			}
		}
	}


	ImGui::PopItemWidth();
}

void UImGui_AudioPlaybackNode::AudioEnvelopBind(const UAudioComponent* AudioComp, const float AverageEnvelopeValue, const float MaxEnvelope, const int32 NumWaveInstances)
{
	AudioEnvelopeValue = MaxEnvelope;
}

void UImGui_AudioPlaybackNode::AudioFinishedBind(UAudioComponent* AudioComp)
{
	if (ShouldLoop && previous_item == FMath::FloorToInt(currentitem.CurrentValue))
	{
		AudioPlayer->Play(0);
	}
}
