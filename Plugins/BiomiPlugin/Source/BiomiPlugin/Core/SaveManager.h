// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGUIHUD.h"
#include "UObject/NoExportTypes.h"
#include "SaveManager.generated.h"



USTRUCT()
struct FBiomiScene
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString SceneName = FString();

	UPROPERTY()
	FGraphSave GraphSave = FGraphSave();

	FBiomiScene() {}

	FBiomiScene(const FString& InSceneName, const FGraphSave& InGraphSave)
	{
		SceneName = InSceneName;
		GraphSave = InGraphSave;
	}
};

USTRUCT()
struct FSceneFiles
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FBiomiScene> Scenes = TArray<FBiomiScene>();

	FSceneFiles() {}

	FBiomiScene* GetSceneForName(const FString& InSceneName)
	{
		for (int32 i = 0; i < Scenes.Num(); ++i)
		{
			if (Scenes[i].SceneName == InSceneName)
			{
				return &Scenes[i];
			}
		}
		return nullptr;
	}

	void GetSceneNameArray(TArray<FString>& StringArray) const
	{
		StringArray.SetNum(Scenes.Num());
		for (int32 i = 0; i < Scenes.Num(); ++i)
		{
			StringArray[i] = Scenes[i].SceneName;
		}
	}
};

/**
 *
 */
UCLASS()
class BIOMIPLUGIN_API USaveManager : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SaveScene();

	UFUNCTION(BlueprintCallable)
	void SaveSceneAs(const FString& Name);

	UFUNCTION(BlueprintCallable)
	void GetSavedFilesNames(TArray<FString>& FilesNames);

	UFUNCTION(BlueprintCallable)
	void LoadScene(const FString& SceneName);

private:

	FString GetSavePath() const;

	void AppendNumberToName(FString& SceneName);

	void AddSceneToSave(const FString& Name, const FGraphSave& GraphSave);

};
