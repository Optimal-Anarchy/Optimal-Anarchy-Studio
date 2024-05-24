// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorNode.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BiomiPlugin/Interfaces/ActorInterface.h"

UActorNode::UActorNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PopUpGenericName = FText::FromString("Actor Node");

	ManualInputValues.SetNum(12);

	// Material Emm Intensity, R,G,B ( 0,1,2,3 )
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Emissive Intensity"), 0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("R"), 0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("G"), 0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("B"), 0.f));
	
	ManualInputValues[0] = &Color_R;
	ManualInputValues[1] = &Color_G;
	ManualInputValues[2] = &Color_B;


	// Translation X,Y,Z ( 4,5,6 )
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Translation_X"), 0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Translation_Y"), 0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Translation_Z"), 0.f));

	ManualInputValues[3] = &Translation_X;
	ManualInputValues[4] = &Translation_Y;
	ManualInputValues[5] = &Translation_Z;

	// Rotation X,Y,Z ( 7,8,9 )
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Rotation_X"), 0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Rotation_Y"), 0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Rotation_Z"), 0.f));

	ManualInputValues[6] = &Rotation_X;
	ManualInputValues[7] = &Rotation_Y;
	ManualInputValues[8] = &Rotation_Z;


	// Scale X,Y,Z ( 10,11,12 )
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Scale.X"), 0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Scale.Y"), 0.f));
	InputAttributes.Add(FImGUIScalarInputAttribute(FText::FromString("Scale.Z"), 0.f));

	ManualInputValues[9] = &Scale_X;
	ManualInputValues[10] = &Scale_Y;
	ManualInputValues[11] = &Scale_Z;

}


void UActorNode::BeginPlay()
{
	Super::BeginPlay();

	//Check if Actor Class is valid then spawn the actor
	if (ActorClass)
	{
		ActorReference = GetWorld()->SpawnActor<AActor>(ActorClass, FTransform());
	}
}

void UActorNode::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Check if Actor is Being Dragged
	bool bIsActorBeingDragged = false;

	//Check If Actor Reference is valid
	if (ActorReference)
	{
		if (ActorReference->Implements<UActorInterface>())
		{
			bIsActorBeingDragged = IActorInterface::Execute_IsActorMoving(ActorReference);
		}
	}


#pragma region Material Logic Handling
	// R
	if (IsInputLinked(1))
	{
		Color_R.CurrentValue = InputAttributes[1].AttributeValue;
	}
	


	// G
	if (IsInputLinked(2))
	{
		Color_G.CurrentValue = InputAttributes[2].AttributeValue;
	}



	// B
	if (IsInputLinked(3))
	{
		Color_B.CurrentValue = InputAttributes[3].AttributeValue;
	}
	
	// Set Emmissive Value and RGB Value in Actor if valid
	if (ActorReference)
	{
		if (ActorReference->Implements<UActorInterface>())
		{
			IActorInterface::Execute_SetEmmisiveIntensity(ActorReference, InputAttributes[0].AttributeValue);
			IActorInterface::Execute_SetMaterialColor(ActorReference, FVector(Color_R.CurrentValue, Color_G.CurrentValue, Color_B.CurrentValue));
		}
	}



#pragma endregion

#pragma region Translation Logic Handling
	// Translation.X
	if (IsInputLinked(4))
	{
		Translation_X.CurrentValue = InputAttributes[4].AttributeValue;
	}
	else
	{
		if (bIsActorBeingDragged)
		{
			Translation_X.CurrentValue = ActorReference->GetActorLocation().X;
		}
	}

	// Translation.Y
	if (IsInputLinked(5))
	{
		Translation_Y.CurrentValue = InputAttributes[5].AttributeValue;
	}
	else
	{
		if (bIsActorBeingDragged)
		{
			Translation_Y.CurrentValue = ActorReference->GetActorLocation().Y;
		}
	}

	// Translation.Z
	if (IsInputLinked(6))
	{
		Translation_Z.CurrentValue = InputAttributes[6].AttributeValue;
	}
	else
	{
		if (bIsActorBeingDragged)
		{
			Translation_Z.CurrentValue = ActorReference->GetActorLocation().Z;
		}
	}


	// Set Actor Location
	if (ActorReference)
	{
		if (!bIsActorBeingDragged)
		{
			ActorReference->SetActorLocation(FVector(Translation_X.CurrentValue, Translation_Y.CurrentValue, Translation_Z.CurrentValue));
		}
	}

#pragma endregion

#pragma region Rotation Logic Handling

	// Rotation.X
	if (IsInputLinked(7))
	{
		Rotation_X.CurrentValue = InputAttributes[7].AttributeValue;
	}
	else
	{
		if (bIsActorBeingDragged)
		{
			Rotation_X.CurrentValue = ActorReference->GetActorRotation().Roll;
		}
	}

	// Rotation.Y
	if (IsInputLinked(8))
	{
		Rotation_Y.CurrentValue = InputAttributes[8].AttributeValue;
	}
	else
	{
		if (bIsActorBeingDragged)
		{
			Rotation_Y.CurrentValue = ActorReference->GetActorRotation().Pitch;
		}
	}

	// Rotation.Z
	if (IsInputLinked(9))
	{
		Rotation_Z.CurrentValue = InputAttributes[9].AttributeValue;
	}
	else
	{
		if (bIsActorBeingDragged)
		{
			Rotation_Z.CurrentValue = ActorReference->GetActorRotation().Yaw;
		}
	}


	// Set Actor Rotation
	if (ActorReference)
	{
		if (!bIsActorBeingDragged)
		{
			ActorReference->SetActorRotation(FRotator(Rotation_Y.CurrentValue, Rotation_Z.CurrentValue, Rotation_X.CurrentValue));
		}
	}

#pragma endregion

#pragma region Actor Scale Handling
	// Scale.X
	if (IsInputLinked(10))
	{
		Scale_X.CurrentValue = InputAttributes[10].AttributeValue;
	}
	else
	{
		if (bIsActorBeingDragged)
		{
			Scale_X.CurrentValue = ActorReference->GetActorScale().X;
		}
	}

	// Scale.Y
	if (IsInputLinked(11))
	{
		Scale_Y.CurrentValue = InputAttributes[11].AttributeValue;
	}
	else
	{
		if (bIsActorBeingDragged)
		{
			Scale_Z.CurrentValue = ActorReference->GetActorScale().Y;
		}
	}

	// Scale.Z
	if (IsInputLinked(12))
	{
		Scale_Z.CurrentValue = InputAttributes[12].AttributeValue;
	}
	else
	{
		if (bIsActorBeingDragged)
		{
			Scale_Z.CurrentValue = ActorReference->GetActorScale().Z;
		}
	}


	// Set Actor Scale
	if (ActorReference)
	{
		if (!bIsActorBeingDragged)
		{
			ActorReference->SetActorScale3D(FVector(Scale_X.CurrentValue, Scale_Y.CurrentValue, Scale_Z.CurrentValue));
		}
	}

#pragma endregion
}

void UActorNode::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (ActorReference)
	{
		ActorReference->Destroy();
	}
}

void UActorNode::ComputeNode()
{

	
}

void UActorNode::NodeImGUICode()
{
	Super::NodeImGUICode();


#pragma region Material Section
	// Emissive Intensity
	AddInputAttribute(0);


	//Material Color Section
	ImGui::NewLine();
	ImGui::TextUnformatted("Material Color");

	// Color.R
	AddInputAttribute(1);
	AddDragFloat("##Color.R", &Color_R.CurrentValue);

	

	// Color.G
	AddInputAttribute(2);
	AddDragFloat("##Color.G", &Color_G.CurrentValue);

	


	// Color.B
	AddInputAttribute(3);
	AddDragFloat("##Color.B", &Color_B.CurrentValue);
#pragma endregion
	

	ImGui::NewLine();

#pragma region Actor Translation
	// Actor Translation Section
	ImGui::TextUnformatted("Actor Translation");


	AddInputAttribute(4,"X");
	AddDragFloat("##Translation.X", &Translation_X.CurrentValue);

	AddInputAttribute(5,"Y");
	AddDragFloat("##Translation.Y", &Translation_Y.CurrentValue);

	AddInputAttribute(6 ,"Z");
	AddDragFloat("##Translation.Z", &Translation_Z.CurrentValue);
#pragma endregion


	ImGui::NewLine();

#pragma region Actor Rotation

	ImGui::TextUnformatted("Actor Rotation");

	//Rotation X
	AddInputAttribute(7,"X");
	AddDragFloat("##Rotation.X", &Rotation_X.CurrentValue);

	//Rotation Y
	AddInputAttribute(8,"Y");
	AddDragFloat("##Rotation.Y", &Rotation_Y.CurrentValue);
	//Rotation Z
	AddInputAttribute(9,"Z");
	AddDragFloat("##Rotation.Z", &Rotation_Z.CurrentValue);

#pragma endregion


	ImGui::NewLine();

#pragma region Actor Scale

	// Actor Translation Section
	ImGui::TextUnformatted("Actor Scale");


	//Scale X
	AddInputAttribute(10, "X");
	AddDragFloat("##Scale.X", &Scale_X.CurrentValue);

	//Scale Y
	AddInputAttribute(11, "Y");
	AddDragFloat("##Scale.Y", &Scale_Y.CurrentValue);

	//Scale Z
	AddInputAttribute(12, "Z");
	AddDragFloat("##Scale.Z", &Scale_Z.CurrentValue);

#pragma endregion

}

void UActorNode::ExecuteImGUICode()
{
	ImNodes::PushColorStyle(ImNodesCol_NodeOutline, ImNodes::IsNodeSelected(NodeID) || ImNodes::IsNodeHovered(&NodeID) ? IM_COL32(0, 255, 255, 255) : IM_COL32(50, 50, 50, 255));
	ImNodes::BeginNode(NodeID);
	NodeImGUICode();

	for (int32 i = 0; i < OutputAttributes.Num(); ++i)
	{
		if (OutputAttributes.IsValidIndex(i))
		{
			if (InputAttributes.IsValidIndex(i))
			{
				ImGui::SameLine(WindowSize.X - ImGui::CalcTextSize(TCHAR_TO_ANSI(*OutputAttributes[i].AttributeName.ToString())).x);
			}
			else
			{
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + WindowSize.X - ImGui::CalcTextSize(TCHAR_TO_ANSI(*OutputAttributes[i].AttributeName.ToString())).x);
			}
			ImNodes::BeginOutputAttribute(OutputAttributes[i].AttributeID, 2);
			ImGui::Text(TCHAR_TO_ANSI(*OutputAttributes[i].AttributeName.ToString()));
			ImNodes::EndOutputAttribute();
		}
	}
	ImNodes::PopColorStyle();
	ImNodes::EndNode();
}

void UActorNode::AddInputAttribute(int i, FString CustomName)
{
	if (InputAttributes.IsValidIndex(i))
	{
		ImNodes::BeginInputAttribute(InputAttributes[i].AttributeID, 5);
		ImGui::Text(TCHAR_TO_ANSI(!CustomName.IsEmpty() ? *CustomName : *InputAttributes[i].AttributeName.ToString()));
		ImNodes::EndInputAttribute();
	}
}

bool UActorNode::AddDragFloat(const char* label, float* value)
{
	ImGui::PushItemWidth(50.f);
	ImGui::SameLine(WindowSize.X - (ImGui::CalcItemWidth() + ImGui::CalcTextSize("").x));
	bool bInteracted = ImGui::DragFloat(label, value);
	ImGui::PopItemWidth();
	return bInteracted;
}
