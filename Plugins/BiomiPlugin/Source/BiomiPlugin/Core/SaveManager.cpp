// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveManager.h"
#include "BiomiPlugin/BiomiPlugin.h"
#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"


void USaveManager::SaveScene()
{
	SaveSceneAs(UGameplayStatics::GetCurrentLevelName(this));
}

void USaveManager::SaveSceneAs(const FString& Name)
{
	if (AImGUIHUD* HUDRef = Cast<AImGUIHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		FGraphSave GraphSave = FGraphSave();
		HUDRef->BuildSaveStruct(GraphSave);
		FString FileSaveName = Name;
		AppendNumberToName(FileSaveName);
		AddSceneToSave(FileSaveName, GraphSave);
	}
	else
	{
		UE_LOG(GeneralImGUILog, Warning, TEXT("Ensure that the HUD class is a AImGUIHUD, see USaveManager::SaveScene"));
	}
}

void USaveManager::AddSceneToSave(const FString& Name, const FGraphSave& GraphSave)
{
	FString ScenesJson = FString();
	FFileHelper::LoadFileToString(ScenesJson, *GetSavePath());
	FSceneFiles SceneFiles = FSceneFiles();
	if (!ScenesJson.IsEmpty())
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(ScenesJson, &SceneFiles);
	}
	SceneFiles.Scenes.Add(FBiomiScene(Name, GraphSave));
	ScenesJson.Empty();

	FJsonObjectConverter::UStructToJsonObjectString(SceneFiles, ScenesJson);
	FFileHelper::SaveStringToFile(ScenesJson, *GetSavePath());
}

void USaveManager::GetSavedFilesNames(TArray<FString>& FilesNames)
{
	FilesNames.Empty();
	FString ScenesJson = FString();
	FFileHelper::LoadFileToString(ScenesJson, *GetSavePath());
	if (!ScenesJson.IsEmpty())
	{
		FSceneFiles SceneFiles = FSceneFiles();
		FJsonObjectConverter::JsonObjectStringToUStruct(ScenesJson, &SceneFiles);
		SceneFiles.GetSceneNameArray(FilesNames);
	}
}

void USaveManager::AppendNumberToName(FString& SceneName)
{
	TArray<FString> FilesNames = TArray<FString>();
	GetSavedFilesNames(FilesNames);
	int32 FileNumber = 0;
	for (int32 i = FilesNames.Num() - 1; i >= 0; --i)
	{
		TArray<FString> ParsedFileName = TArray<FString>();
		FilesNames[i].ParseIntoArray(ParsedFileName, TEXT("_"), false);
		FString FileNameWithoutNumber = FString();
		for (int32 y = 0; y < ParsedFileName.Num() - 1; ++y)
		{
			FileNameWithoutNumber += ParsedFileName[y];
			if (y < ParsedFileName.Num() - 2)
			{
				FileNameWithoutNumber += "_";
			}
		}
		if (FileNameWithoutNumber == SceneName)
		{
			if (ParsedFileName.Num() > 0 && ParsedFileName.Top().IsNumeric())
			{
				FileNumber = FCString::Atoi(*ParsedFileName.Top());
				FileNumber++;
			}
			break;
		}
	}

	FNumberFormattingOptions NumberFormat = FNumberFormattingOptions::DefaultNoGrouping();
	NumberFormat.SetMinimumIntegralDigits(2);
	NumberFormat.SetMaximumIntegralDigits(12);
	SceneName += "_" + FText::AsNumber(FileNumber, &NumberFormat).ToString();
}

void USaveManager::LoadScene(const FString& SceneName)
{
	if (AImGUIHUD* HUDRef = Cast<AImGUIHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		FSceneFiles SceneFiles = FSceneFiles();
		FString JsonSceneFiles = FString();
		if (FFileHelper::LoadFileToString(JsonSceneFiles, *GetSavePath()))
		{
			FJsonObjectConverter::JsonObjectStringToUStruct(JsonSceneFiles, &SceneFiles);
			FBiomiScene* SceneRetrieved = SceneFiles.GetSceneForName(SceneName);
			if (SceneRetrieved)
			{
				HUDRef->LoadSaveStruct(SceneRetrieved->GraphSave);
			}
		}		
	}
	else
	{
		UE_LOG(GeneralImGUILog, Warning, TEXT("Ensure that the HUD class is a AImGUIHUD, see USaveManager::LoadScene"));
	}
}

FString USaveManager::GetSavePath() const
{
	return (FPaths::ProjectSavedDir() + "/SavedScenes/Scenes.json");
}