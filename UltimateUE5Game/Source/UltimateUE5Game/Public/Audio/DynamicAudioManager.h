// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynamicAudioManager.generated.h"

UENUM(BlueprintType)
enum class EAudioState : uint8
{
	Explore UMETA(DisplayName = "Explore"),
	Stealth UMETA(DisplayName = "Stealth"),
	Combat UMETA(DisplayName = "Combat"),
	Cinematic UMETA(DisplayName = "Cinematic"),
	Chase UMETA(DisplayName = "Chase")
};

/**
 * Dynamic Audio Manager
 * Handles adaptive music, sound states, reverb zones, and cinematic audio
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ULTIMATEUE5GAME_API UDynamicAudioManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UDynamicAudioManager();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Audio State Management
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetAudioState(EAudioState NewState);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	EAudioState GetCurrentAudioState() const { return CurrentState; }

	// Cinematic Audio
	UFUNCTION(BlueprintCallable, Category = "Audio|Cinematic")
	void PlayCinematicSound(USoundBase* Sound, float VolumeMultiplier = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Audio|Cinematic")
	void StopCinematicSounds();

	// Reverb & Effects
	UFUNCTION(BlueprintCallable, Category = "Audio|Effects")
	void ApplyReverbEffect(UReverbEffect* Reverb, float FadeTime = 2.0f);

	UFUNCTION(BlueprintCallable, Category = "Audio|Effects")
	void ClearReverbEffect(float FadeTime = 2.0f);

	// Dynamic Music
	UFUNCTION(BlueprintCallable, Category = "Audio|Music")
	void SetMusicIntensity(float Intensity); // 0.0 to 1.0

	UFUNCTION(BlueprintCallable, Category = "Audio|Music")
	void PlayMusicTransition(const FString& TransitionName);

	// Spatial Audio
	UFUNCTION(BlueprintCallable, Category = "Audio|Spatial")
	void SetPlayerPositionForAudio(const FVector& Location);

protected:
	UPROPERTY(EditAnywhere, Category = "Audio")
	EAudioState CurrentState;

	UPROPERTY(EditAnywhere, Category = "Audio")
	float CurrentIntensity;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> AmbientAudioComponent;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> MusicAudioComponent;

private:
	void TransitionToState(EAudioState NewState);
	void UpdateMusicLayering();
	void ApplyReverbToWorld(UReverbEffect* Reverb, float FadeTime);
};
