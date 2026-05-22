// Copyright 2024 Ultimate Game Studio. All Rights Reserved.

#include "Survival/USurvivalComponent.h"
#include "GameFramework/Actor.h"
#include "Combat/UHealthComponent.h"

USurvivalComponent::USurvivalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	// Stamina defaults
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
	StaminaRegenRate = 10.0f;
	SprintStaminaDrain = 15.0f;
	DashStaminaCost = 25.0f;
	
	// Hunger defaults
	MaxHunger = 100.0f;
	CurrentHunger = MaxHunger;
	HungerDecayRate = 2.0f;
	StarvationDamage = 5.0f;
	
	// Thirst defaults
	MaxThirst = 100.0f;
	CurrentThirst = MaxThirst;
	ThirstDecayRate = 3.0f;
	DehydrationDamage = 5.0f;
	
	// Temperature defaults
	NormalBodyTemp = 36.6f;
	CurrentBodyTemp = NormalBodyTemp;
	MinSafeTemp = 35.0f;
	MaxSafeTemp = 38.0f;
	TempAdjustmentRate = 0.5f;
}

void USurvivalComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeComponent();
}

void USurvivalComponent::InitializeComponent()
{
	CurrentStamina = MaxStamina;
	CurrentHunger = MaxHunger;
	CurrentThirst = MaxThirst;
	CurrentBodyTemp = NormalBodyTemp;
	
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina, 0.0f);
	OnHungerChanged.Broadcast(CurrentHunger, MaxHunger, 0.0f);
	OnThirstChanged.Broadcast(CurrentThirst, MaxThirst, 0.0f);
	OnTemperatureChanged.Broadcast(CurrentBodyTemp, NormalBodyTemp, 0.0f);
}

bool USurvivalComponent::HasEnoughStaminaForSprint() const
{
	return CurrentStamina >= SprintStaminaDrain * 0.1f; // Need at least some stamina
}

bool USurvivalComponent::HasEnoughStaminaForDash() const
{
	return CurrentStamina >= DashStaminaCost;
}

bool USurvivalComponent::ConsumeStamina(float Amount)
{
	if (CurrentStamina < Amount)
	{
		return false;
	}
	
	float OldStamina = CurrentStamina;
	CurrentStamina -= Amount;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina, CurrentStamina - OldStamina);
	return true;
}

void USurvivalComponent::AddStamina(float Amount)
{
	float OldStamina = CurrentStamina;
	CurrentStamina += Amount;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina, CurrentStamina - OldStamina);
}

void USurvivalComponent::EatFood(float FoodValue)
{
	float OldHunger = CurrentHunger;
	CurrentHunger += FoodValue;
	CurrentHunger = FMath::Clamp(CurrentHunger, 0.0f, MaxHunger);
	
	OnHungerChanged.Broadcast(CurrentHunger, MaxHunger, CurrentHunger - OldHunger);
}

void USurvivalComponent::DrinkWater(float WaterValue)
{
	float OldThirst = CurrentThirst;
	CurrentThirst += WaterValue;
	CurrentThirst = FMath::Clamp(CurrentThirst, 0.0f, MaxThirst);
	
	OnThirstChanged.Broadcast(CurrentThirst, MaxThirst, CurrentThirst - OldThirst);
}

void USurvivalComponent::AdjustTemperature(float ExternalTemp, float DeltaTime)
{
	float OldTemp = CurrentBodyTemp;
	
	// Adjust body temperature towards external temperature
	if (ExternalTemp > CurrentBodyTemp)
	{
		CurrentBodyTemp += FMath::Min(ExternalTemp - CurrentBodyTemp, TempAdjustmentRate * DeltaTime);
	}
	else
	{
		CurrentBodyTemp -= FMath::Min(CurrentBodyTemp - ExternalTemp, TempAdjustmentRate * DeltaTime);
	}
	
	OnTemperatureChanged.Broadcast(CurrentBodyTemp, NormalBodyTemp, CurrentBodyTemp - OldTemp);
}

bool USurvivalComponent::IsStarving() const
{
	return CurrentHunger <= 10.0f;
}

bool USurvivalComponent::IsDehydrated() const
{
	return CurrentThirst <= 10.0f;
}

bool USurvivalComponent::IsOverheating() const
{
	return CurrentBodyTemp > MaxSafeTemp;
}

bool USurvivalComponent::IsFreezing() const
{
	return CurrentBodyTemp < MinSafeTemp;
}

void USurvivalComponent::HandleStaminaRegen(float DeltaTime)
{
	// Only regenerate if not sprinting and have room
	if (CurrentStamina < MaxStamina)
	{
		AddStamina(StaminaRegenRate * DeltaTime);
	}
}

void USurvivalComponent::HandleHungerDecay(float DeltaTime)
{
	float OldHunger = CurrentHunger;
	CurrentHunger -= HungerDecayRate * DeltaTime;
	CurrentHunger = FMath::Clamp(CurrentHunger, 0.0f, MaxHunger);
	
	if (FMath::Abs(CurrentHunger - OldHunger) > 0.001f)
	{
		OnHungerChanged.Broadcast(CurrentHunger, MaxHunger, CurrentHunger - OldHunger);
	}
	
	// Apply starvation damage
	if (IsStarving())
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			UHealthComponent* HealthComp = Owner->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				HealthComp->TakeDamage(StarvationDamage * DeltaTime, nullptr, Owner);
			}
		}
	}
}

void USurvivalComponent::HandleThirstDecay(float DeltaTime)
{
	float OldThirst = CurrentThirst;
	CurrentThirst -= ThirstDecayRate * DeltaTime;
	CurrentThirst = FMath::Clamp(CurrentThirst, 0.0f, MaxThirst);
	
	if (FMath::Abs(CurrentThirst - OldThirst) > 0.001f)
	{
		OnThirstChanged.Broadcast(CurrentThirst, MaxThirst, CurrentThirst - OldThirst);
	}
	
	// Apply dehydration damage
	if (IsDehydrated())
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			UHealthComponent* HealthComp = Owner->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				HealthComp->TakeDamage(DehydrationDamage * DeltaTime, nullptr, Owner);
			}
		}
	}
}

void USurvivalComponent::HandleTemperatureEffects(float DeltaTime)
{
	// Apply damage if temperature is unsafe
	if (IsOverheating() || IsFreezing())
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			UHealthComponent* HealthComp = Owner->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				float TempDamage = FMath::Abs(CurrentBodyTemp - NormalBodyTemp) * 0.5f;
				HealthComp->TakeDamage(TempDamage * DeltaTime, nullptr, Owner);
			}
		}
	}
}

void USurvivalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	HandleStaminaRegen(DeltaTime);
	HandleHungerDecay(DeltaTime);
	HandleThirstDecay(DeltaTime);
	// HandleTemperatureEffects(DeltaTime); // Uncomment when weather system provides external temp
}
