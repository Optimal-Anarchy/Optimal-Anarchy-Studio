// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUIAudioNode.h"
#include "Components/AudioComponent.h"

UImGUIAudioNode::UImGUIAudioNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OutputAttributes.Add(FImGUIScalarOutputAttribute(FText::FromString("Output")));

	ManualInputValues.SetNum(18);
	ManualInputValues[0] = &Frequency_V1;
	ManualInputValues[1] = &BinauralBeatFreq_V1;
	ManualInputValues[2] = &MIX_L_R_V1;
	ManualInputValues[3] = &Boost_V1;
	ManualInputValues[4] = &Attack_Time_V1;
	ManualInputValues[5] = &Decay_Time_V1;
	ManualInputValues[6] = &Attack_Curve_V1;
	ManualInputValues[7] = &Decay_Curve_V1;
	ManualInputValues[8] = &Frequency_V2;
	ManualInputValues[9] = &BinauralBeatFreq_V2;
	ManualInputValues[10] = &MIX_L_R_V2;
	ManualInputValues[11] = &Boost_V2;
	ManualInputValues[12] = &Attack_Time_V2;
	ManualInputValues[13] = &Decay_Time_V2;
	ManualInputValues[14] = &Attack_Curve_V2;
	ManualInputValues[15] = &Decay_Curve_V2;
	ManualInputValues[16] = &Gain_0;
	ManualInputValues[17] = &Gain_1_1;

    WindowSize = FVector2D(300.f, 100.f);

    if (GetOwner())
    {
        TArray<UAudioComponent*> AudioComponents;
        GetOwner()->GetComponents<UAudioComponent>(AudioComponents);

        for (int32 i = 0; i < AudioComponents.Num(); ++i)
        {
            if (AudioComponents[i])
            {
                AudioComponents[i]->OnAudioMultiEnvelopeValueNative.AddUObject(this, &UImGUIAudioNode::AudioEnvelopBind);
            }
        }
    }
}

void UImGUIAudioNode::AudioEnvelopBind(const UAudioComponent* AudioComp, const float AverageEnvelopeValue, const float MaxEnvelope, const int32 NumWaveInstances)
{
    Envelopes.FindOrAdd(AudioComp) = AverageEnvelopeValue;
}
   
void UImGUIAudioNode::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Envelopes.Num() > 0)
    {
        float TotalEnvelop = 0.f;
        for (auto& Audio : Envelopes)
        {
            TotalEnvelop += Audio.Value;
        }
        OutputAttributes[0].AttributeValue = TotalEnvelop / Envelopes.Num();
    }
}

void UImGUIAudioNode::ComputeNode()
{
	Super::ComputeNode();
    TArray<UAudioComponent*> AudioComponents;
    GetOwner()->GetComponents<UAudioComponent>(AudioComponents);

    for (int32 i = 0; i < AudioComponents.Num(); i++)
    {
        AudioComponents[i]->SetFloatParameter(FName("Fundamental_Frequency_V1"), Frequency_V1.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Binaural_Beat_Freq_V1"), BinauralBeatFreq_V1.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("MIX_L_R_V1"), MIX_L_R_V1.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Boost_V1"), Boost_V1.CurrentValue);

        AudioComponents[i]->SetFloatParameter(FName("Attack_Time_V1"), Attack_Time_V1.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Decay_Time_V1"), Decay_Time_V1.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Attack_Curve_V1"), Attack_Curve_V1.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Decay_Curve_V2"), Decay_Curve_V1.CurrentValue);

        AudioComponents[i]->SetFloatParameter(FName("Fundamental_Frequency_V2"), Frequency_V2.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Binaural_Beat_Freq_V2"), BinauralBeatFreq_V2.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("MIX_L_R_V2"), MIX_L_R_V2.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Boost_V2"), Boost_V2.CurrentValue);

        AudioComponents[i]->SetFloatParameter(FName("Attack_Time_V2"), Attack_Time_V2.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Decay_Time_V2"), Decay_Time_V2.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Attack_Curve_V2"), Attack_Curve_V2.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Decay_Curve_V2"), Decay_Curve_V2.CurrentValue);

        AudioComponents[i]->SetFloatParameter(FName("Voice 1"), Gain_0.CurrentValue);
        AudioComponents[i]->SetFloatParameter(FName("Voice 2"), Gain_1_1.CurrentValue);
    }
}

void UImGUIAudioNode::NodeImGUICode()
{
	Super::NodeImGUICode();

    ImGui::PushItemWidth(50.f);
    
    if (ImGui::TreeNode("Voice 1", WindowSize.X))
    {
        ImGui::DragFloat("Fundamental_Frequency_V1", &Frequency_V1.CurrentValue,1.f ,0.0f, 990.0f,"%.3f",ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat("Binaural_Beat_Freq_V1", &BinauralBeatFreq_V1.CurrentValue,1.f ,-40.0f, 40.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat("MIX_L_R_V1", &MIX_L_R_V1.CurrentValue,1.f ,0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat("Boost_V1", &Boost_V1.CurrentValue,1.f, 0.0f, 5.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);


        if (ImGui::TreeNode("Delay", WindowSize.X - 20.f))
        {
            ImGui::DragFloat("Attack_Time_V1", &Attack_Time_V1.CurrentValue, 1.f,0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::DragFloat("Decay Time", &Decay_Time_V1.CurrentValue,1.f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::DragFloat("Attack Curve", &Attack_Curve_V1.CurrentValue,1, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::DragFloat("Decay Curve", &Decay_Curve_V1.CurrentValue,1, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

            ImGui::TreePop(); 
        }

        ImGui::Separator();
        ImGui::TreePop(); 
    }

    if (ImGui::TreeNode("Voice 2", WindowSize.X))
    {
        ImGui::DragFloat("Fundamental_Frequency_V2", &Frequency_V2.CurrentValue, 1,0.0f, 990.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat("Binaural_Beat_Freq_V2", &BinauralBeatFreq_V2.CurrentValue,1, -40.0f, 40.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat("MIX_L_R_V2", &MIX_L_R_V2.CurrentValue,1, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat("Boost_V2", &Boost_V2.CurrentValue, 1 ,0.0f, 5.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

        if (ImGui::TreeNode("Delay", WindowSize.X - 20.f))
        {
            ImGui::DragFloat("Attack_Time_V2", &Attack_Time_V2.CurrentValue,1, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::DragFloat("Decay_Time_V2", &Decay_Time_V2.CurrentValue,1, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::DragFloat("Attack_Curve_V2", &Attack_Curve_V2.CurrentValue,1, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::DragFloat("Decay_Curve_V2", &Decay_Curve_V2.CurrentValue, 1,0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

            ImGui::TreePop(); 
        }

        ImGui::Separator();
        ImGui::TreePop(); 
    }

    if (ImGui::TreeNode("MIX", WindowSize.X))
    {
        ImGui::DragFloat("Voice 1", &Gain_0.CurrentValue,1 ,0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat("Voice 2", &Gain_1_1.CurrentValue, 1,0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

        ImGui::TreePop();
    }

    ImGui::PopItemWidth();
}