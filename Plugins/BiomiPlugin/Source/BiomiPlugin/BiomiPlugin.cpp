// Copyright Epic Games, Inc. All Rights Reserved.

#include "BiomiPlugin.h"

#define LOCTEXT_NAMESPACE "FBiomiPluginModule"
DEFINE_LOG_CATEGORY(GeneralImGUILog);

void FBiomiPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FBiomiPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBiomiPluginModule, BiomiPlugin)