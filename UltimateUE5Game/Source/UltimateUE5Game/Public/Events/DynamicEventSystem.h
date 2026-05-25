// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DynamicEventSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEventTriggered, const FString&, EventName, const FVariant&, EventData);

/**
 * Dynamic Event Data Structure
 */
USTRUCT(BlueprintType)
struct FDynamicEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FString EventName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FString EventType; // Weather, Encounter, Quest, WorldChange

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	float Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	TMap<FString, float> Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	TMap<FString, FString> Parameters;

	UFUNCTION(BlueprintCallable, Category = "Event")
	bool CanTrigger(const TMap<FString, float>& WorldState) const;
};

/**
 * Dynamic Event System
 * Manages random events, world changes, and emergent gameplay
 */
UCLASS()
class ULTIMATEUE5GAME_API UDynamicEventSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;

	// Event Management
	UFUNCTION(BlueprintCallable, Category = "Events")
	void RegisterEvent(const FDynamicEvent& Event);

	UFUNCTION(BlueprintCallable, Category = "Events")
	void TriggerEvent(const FString& EventName, const FVariant& Data = FVariant());

	UFUNCTION(BlueprintCallable, Category = "Events")
	void CancelEvent(const FString& EventName);

	// Event Queries
	UFUNCTION(BlueprintCallable, Category = "Events")
	bool IsEventActive(const FString& EventName) const;

	UFUNCTION(BlueprintCallable, Category = "Events")
	TArray<FString> GetActiveEvents() const;

	// World State
	UFUNCTION(BlueprintCallable, Category = "Events")
	void UpdateWorldState(const FString& Key, float Value);

	UFUNCTION(BlueprintCallable, Category = "Events")
	float GetWorldState(const FString& Key) const;

	// Event Callbacks
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEventTriggered OnEventTriggered;

protected:
	UPROPERTY(EditAnywhere, Category = "Events")
	TArray<FDynamicEvent> AvailableEvents;

	UPROPERTY()
	TMap<FString, FDynamicEvent> ActiveEvents;

	UPROPERTY()
	TMap<FString, float> WorldState;

private:
	float TimeSinceLastEvent;
	float MinEventInterval;
	float MaxEventInterval;

	void CheckForEventTriggers();
	void ProcessEventEffects(const FDynamicEvent& Event);
	void GenerateRandomEvent();
};
