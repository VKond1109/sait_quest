// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#include "Core/UltimateGameInstance.h"
#include "Audio/DynamicAudioManager.h"
#include "Online/OnlineService.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Internationalization/Internationalization.h"

void UUltimateGameInstance::Init()
{
	Super::Init();

	// Default settings
	bIsColorblindMode = false;
	SubtitleSize = 1.0f;
	MasterVolume = 1.0f;
	MusicVolume = 0.8f;
	SFXVolume = 1.0f;

	InitializeSubsystems();

	UE_LOG(LogTemp, Log, TEXT("UltimateGameInstance Initialized"));
}

void UUltimateGameInstance::Shutdown()
{
	Super::Shutdown();
	UE_LOG(LogTemp, Log, TEXT("UltimateGameInstance Shutdown"));
}

void UUltimateGameInstance::InitializeSubsystems()
{
	// Initialize Audio Manager
	if (GetWorld())
	{
		AActor* TempActor = GetWorld()->SpawnActor<AActor>();
		if (TempActor)
		{
			AudioManager = TempActor->AddComponentByClass<UDynamicAudioManager>(false, FTransform::Identity, false);
		}
	}

	// Initialize Online Service
	OnlineService = NewObject<UOnlineService>(this);
	if (OnlineService)
	{
		OnlineService->Initialize(this);
	}
}

void UUltimateGameInstance::PauseGame()
{
	if (GetFirstLocalPlayerController())
	{
		GetFirstLocalPlayerController()->SetPause(true);
		UE_LOG(LogTemp, Log, TEXT("Game Paused"));
	}
}

void UUltimateGameInstance::ResumeGame()
{
	if (GetFirstLocalPlayerController())
	{
		GetFirstLocalPlayerController()->SetPause(false);
		UE_LOG(LogTemp, Log, TEXT("Game Resumed"));
	}
}

void UUltimateGameInstance::SaveGame(const FString& SlotName)
{
	UGameplayStatics::SaveGameToSlot(CreateSaveGameObject(), SlotName, 0);
	UE_LOG(LogTemp, Log, TEXT("Game Saved to slot: %s"), *SlotName);
}

void UUltimateGameInstance::LoadGame(const FString& SlotName)
{
	USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
	if (LoadedGame)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Loaded from slot: %s"), *SlotName);
	}
}

void UUltimateGameInstance::SaveToCloud()
{
	if (OnlineService)
	{
		OnlineService->SaveCloudData();
	}
}

void UUltimateGameInstance::LoadFromCloud()
{
	if (OnlineService)
	{
		OnlineService->LoadCloudData();
	}
}

void UUltimateGameInstance::TrackEvent(const FString& EventName, const TMap<FString, FString>& Params)
{
	FString LogMsg = TEXT("Analytics Event: ") + EventName;
	for (auto& Param : Params)
	{
		LogMsg += FString::Printf(TEXT(" | %s=%s"), *Param.Key, *Param.Value);
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMsg);
	
	// Here you would integrate with GameAnalytics, Firebase, etc.
}

void UUltimateGameInstance::TrackProgression(const FString& QuestID, const FString& State)
{
	TMap<FString, FString> Params;
	Params.Add("QuestID", QuestID);
	Params.Add("State", State);
	TrackEvent("QuestProgression", Params);
}

void UUltimateGameInstance::SetLanguage(const FString& CultureName)
{
	FInternationalization::Get().SetCurrentCulture(CultureName);
	UE_LOG(LogTemp, Log, TEXT("Language set to: %s"), *CultureName);
}

void UUltimateGameInstance::ToggleColorblindMode(bool bEnable)
{
	bIsColorblindMode = bEnable;
	UE_LOG(LogTemp, Log, TEXT("Colorblind mode: %s"), bEnable ? TEXT("Enabled") : TEXT("Disabled"));
	
	// Trigger post-process volume update via event or delegate
}
