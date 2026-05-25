// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#include "Audio/DynamicAudioManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

UDynamicAudioManager::UDynamicAudioManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentState = EAudioState::Explore;
	CurrentIntensity = 0.5f;
}

void UDynamicAudioManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Update music layering based on intensity
	UpdateMusicLayering();
}

void UDynamicAudioManager::SetAudioState(EAudioState NewState)
{
	if (CurrentState != NewState)
	{
		TransitionToState(NewState);
		CurrentState = NewState;
		
		UE_LOG(LogTemp, Log, TEXT("Audio State Changed to: %d"), (uint8)NewState);
	}
}

void UDynamicAudioManager::TransitionToState(EAudioState NewState)
{
	// Smooth transition logic between states
	// In production: crossfade music tracks, change ambient sounds
	
	switch (NewState)
	{
	case EAudioState::Explore:
		SetMusicIntensity(0.3f);
		break;
	case EAudioState::Stealth:
		SetMusicIntensity(0.6f);
		break;
	case EAudioState::Combat:
		SetMusicIntensity(1.0f);
		break;
	case EAudioState::Cinematic:
		SetMusicIntensity(0.0f);
		break;
	case EAudioState::Chase:
		SetMusicIntensity(0.9f);
		break;
	}
}

void UDynamicAudioManager::PlayCinematicSound(USoundBase* Sound, float VolumeMultiplier)
{
	if (!Sound || !GetWorld()) return;
	
	UGameplayStatics::PlaySound2D(GetWorld(), Sound, VolumeMultiplier);
	UE_LOG(LogTemp, Log, TEXT("Playing cinematic sound"));
}

void UDynamicAudioManager::StopCinematicSounds()
{
	if (GetWorld())
	{
		UGameplayStatics::StopAllSoundForPlayer(GetWorld()->GetFirstPlayerController());
	}
}

void UDynamicAudioManager::ApplyReverbEffect(UReverbEffect* Reverb, float FadeTime)
{
	if (!Reverb) return;
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		// Apply reverb through audio volume or direct component
		ApplyReverbToWorld(Reverb, FadeTime);
		UE_LOG(LogTemp, Log, TEXT("Applied reverb effect with fade: %f"), FadeTime);
	}
}

void UDynamicAudioManager::ClearReverbEffect(float FadeTime)
{
	ApplyReverbToWorld(nullptr, FadeTime);
	UE_LOG(LogTemp, Log, TEXT("Cleared reverb effect"));
}

void UDynamicAudioManager::SetMusicIntensity(float Intensity)
{
	CurrentIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
	UpdateMusicLayering();
}

void UDynamicAudioManager::PlayMusicTransition(const FString& TransitionName)
{
	// Trigger MetaSound transition or crossfade
	UE_LOG(LogTemp, Log, TEXT("Playing music transition: %s"), *TransitionName);
}

void UDynamicAudioManager::SetPlayerPositionForAudio(const FVector& Location)
{
	// Update spatial audio parameters
	if (AmbientAudioComponent)
	{
		AmbientAudioComponent->SetWorldLocation(Location);
	}
}

void UDynamicAudioManager::UpdateMusicLayering()
{
	// Adjust volume of different music layers based on intensity
	// Example: percussion layer at 0.7 intensity, strings at 0.4, etc.
}

void UDynamicAudioManager::ApplyReverbToWorld(UReverbEffect* Reverb, float FadeTime)
{
	// Implementation depends on your audio middleware (Wwise, FMOD) or native UE audio
	// Could modify PostProcessVolume or AudioVolume settings
}
