// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#include "Events/DynamicEventSystem.h"
#include "Engine/World.h"

bool FDynamicEvent::CanTrigger(const TMap<FString, float>& CurrentWorldState) const
{
	// Check if all conditions are met
	for (auto& Condition : Conditions)
	{
		if (!CurrentWorldState.Contains(Condition.Key))
		{
			return false;
		}
		
		if (CurrentWorldState[Condition.Key] < Condition.Value)
		{
			return false;
		}
	}
	
	return true;
}

void UDynamicEventSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	TimeSinceLastEvent = 0.0f;
	MinEventInterval = 60.0f; // Minimum 60 seconds between events
	MaxEventInterval = 300.0f; // Maximum 5 minutes
	
	// Initialize default world states
	WorldState.Add("TimeOfDay", 12.0f);
	WorldState.Add("PlayerHealth", 100.0f);
	WorldState.Add("DangerLevel", 0.0f);
	WorldState.Add("PopulationDensity", 0.5f);
	
	UE_LOG(LogTemp, Log, TEXT("DynamicEventSystem Initialized"));
}

void UDynamicEventSystem::Deinitialize()
{
	Super::Deinitialize();
	ActiveEvents.Empty();
	UE_LOG(LogTemp, Log, TEXT("DynamicEventSystem Deinitialized"));
}

void UDynamicEventSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimeSinceLastEvent += DeltaTime;
	
	// Update time of day
	float CurrentTime = GetWorldState("TimeOfDay");
	CurrentTime += DeltaTime / 60.0f; // Convert to hours
	if (CurrentTime >= 24.0f)
	{
		CurrentTime = 0.0f;
	}
	UpdateWorldState("TimeOfDay", CurrentTime);
	
	// Check for event triggers
	if (TimeSinceLastEvent >= MinEventInterval)
	{
		CheckForEventTriggers();
	}
}

void UDynamicEventSystem::RegisterEvent(const FDynamicEvent& Event)
{
	AvailableEvents.Add(Event);
	UE_LOG(LogTemp, Log, TEXT("Registered event: %s"), *Event.EventName);
}

void UDynamicEventSystem::TriggerEvent(const FString& EventName, const FVariant& Data)
{
	FDynamicEvent* FoundEvent = nullptr;
	
	for (FDynamicEvent& Event : AvailableEvents)
	{
		if (Event.EventName == EventName)
		{
			FoundEvent = &Event;
			break;
		}
	}
	
	if (!FoundEvent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Event not found: %s"), *EventName);
		return;
	}
	
	ActiveEvents.Add(EventName, *FoundEvent);
	TimeSinceLastEvent = 0.0f;
	
	ProcessEventEffects(*FoundEvent);
	
	// Broadcast event
	OnEventTriggered.Broadcast(EventName, Data);
	
	UE_LOG(LogTemp, Log, TEXT("Triggered event: %s"), *EventName);
	
	// Schedule event cancellation after duration
	if (FoundEvent->Duration > 0.0f)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateUObject(this, &UDynamicEventSystem::CancelEvent, EventName),
			FoundEvent->Duration,
			false
		);
	}
}

void UDynamicEventSystem::CancelEvent(const FString& EventName)
{
	if (ActiveEvents.Contains(EventName))
	{
		ActiveEvents.Remove(EventName);
		UE_LOG(LogTemp, Log, TEXT("Cancelled event: %s"), *EventName);
	}
}

bool UDynamicEventSystem::IsEventActive(const FString& EventName) const
{
	return ActiveEvents.Contains(EventName);
}

TArray<FString> UDynamicEventSystem::GetActiveEvents() const
{
	TArray<FString> Result;
	ActiveEvents.GetKeys(Result);
	return Result;
}

void UDynamicEventSystem::UpdateWorldState(const FString& Key, float Value)
{
	WorldState.Add(Key, Value);
	
	// Check if this update triggers any events
	CheckForEventTriggers();
}

float UDynamicEventSystem::GetWorldState(const FString& Key) const
{
	if (WorldState.Contains(Key))
	{
		return WorldState[Key];
	}
	return 0.0f;
}

void UDynamicEventSystem::CheckForEventTriggers()
{
	// Find eligible events
	TArray<FDynamicEvent*> EligibleEvents;
	
	for (FDynamicEvent& Event : AvailableEvents)
	{
		if (!ActiveEvents.Contains(Event.EventName) && Event.CanTrigger(WorldState))
		{
			EligibleEvents.Add(&Event);
		}
	}
	
	if (EligibleEvents.Num() > 0)
	{
		// Select random event based on priority
		float TotalPriority = 0.0f;
		for (const FDynamicEvent* Event : EligibleEvents)
		{
			TotalPriority += Event->Priority;
		}
		
		float RandomValue = FMath::FRandRange(0.0f, TotalPriority);
		float CumulativePriority = 0.0f;
		
		for (const FDynamicEvent* Event : EligibleEvents)
		{
			CumulativePriority += Event->Priority;
			if (RandomValue <= CumulativePriority)
			{
				TriggerEvent(Event->EventName);
				break;
			}
		}
	}
}

void UDynamicEventSystem::ProcessEventEffects(const FDynamicEvent& Event)
{
	// Apply event effects to world state or game systems
	for (auto& Param : Event.Parameters)
	{
		UE_LOG(LogTemp, Log, TEXT("Event Parameter: %s = %s"), *Param.Key, *Param.Value);
	}
	
	// Example: modify world state based on event type
	if (Event.EventType == "Weather")
	{
		UpdateWorldState("WeatherIntensity", 1.0f);
	}
	else if (Event.EventType == "Encounter")
	{
		UpdateWorldState("DangerLevel", GetWorldState("DangerLevel") + 0.3f);
	}
}

void UDynamicEventSystem::GenerateRandomEvent()
{
	// Fallback random event generation if no events are registered
	if (AvailableEvents.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("No events registered, skipping random generation"));
		return;
	}
	
	int32 RandomIndex = FMath::RandRange(0, AvailableEvents.Num() - 1);
	TriggerEvent(AvailableEvents[RandomIndex].EventName);
}
