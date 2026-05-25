// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#include "Core/UltimateUE5GameModule.h"

#define LOCTEXT_NAMESPACE "FUltimateUE5GameModule"

void FUltimateUE5GameModule::StartupModule()
{
	// Code that will run when the module starts up
	UE_LOG(LogTemp, Log, TEXT("UltimateUE5Game Module Started"));
}

void FUltimateUE5GameModule::ShutdownModule()
{
	// Code that will run when the module shuts down
	UE_LOG(LogTemp, Log, TEXT("UltimateUE5Game Module Shutdown"));
}

#undef LOCTEXT_NAMESPACE
