// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UltimateGameInstance.generated.h"

/**
 * Main Game Instance for Ultimate UE5 Game
 * Handles global game state, settings, online features, and analytics
 */
UCLASS()
class ULTIMATEUE5GAME_API UUltimateGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	// Accessibility Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	bool bIsColorblindMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	float SubtitleSize;

	// Audio Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float SFXVolume;

	// Game State Management
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Game State")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Game State")
	void SaveGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Game State")
	void LoadGame(const FString& SlotName);

	// Cloud Saves
	UFUNCTION(BlueprintCallable, Category = "Online")
	void SaveToCloud();

	UFUNCTION(BlueprintCallable, Category = "Online")
	void LoadFromCloud();

	// Analytics & Telemetry
	UFUNCTION(BlueprintCallable, Category = "Analytics")
	void TrackEvent(const FString& EventName, const TMap<FString, FString>& Params);

	UFUNCTION(BlueprintCallable, Category = "Analytics")
	void TrackProgression(const FString& QuestID, const FString& State);

	// Language & Localization
	UFUNCTION(BlueprintCallable, Category = "Localization")
	void SetLanguage(const FString& CultureName);

	UFUNCTION(BlueprintCallable, Category = "Accessibility")
	void ToggleColorblindMode(bool bEnable);

protected:
	UPROPERTY()
	TObjectPtr<class UDynamicAudioManager> AudioManager;

	UPROPERTY()
	TObjectPtr<class UOnlineService> OnlineService;

private:
	void InitializeSubsystems();
};
