// Copyright 2024. All rights reserved.

#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
PrimaryComponentTick.bCanEverTick = false;
CurrentHealth = 100.0f;
MaxHealthValue = 100.0f;
bIsInvulnerable = false;
}

void UHealthComponent::BeginPlay()
{
Super::BeginPlay();
CurrentHealth = MaxHealthValue;
}

float UHealthComponent::GetCurrentHealth() const
{
return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
return MaxHealthValue;
}

float UHealthComponent::GetHealthPercent() const
{
return CurrentHealth / MaxHealthValue;
}

void UHealthComponent::SetMaxHealth(float NewMaxHealth)
{
MaxHealthValue = FMath::Max(NewMaxHealth, 1.0f);
CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealthValue);
OnHealthChanged.Broadcast(CurrentHealth);
}

void UHealthComponent::TakeDamage(float Damage)
{
if (bIsInvulnerable || Damage <= 0.0f) return;

CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealthValue);
OnHealthChanged.Broadcast(CurrentHealth);

if (CurrentHealth <= 0.0f)
{
OnHealthDepleted.Broadcast();
}
}

void UHealthComponent::Heal(float Amount)
{
if (Amount <= 0.0f) return;

CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealthValue);
OnHealthChanged.Broadcast(CurrentHealth);
}

void UHealthComponent::Kill()
{
CurrentHealth = 0.0f;
OnHealthChanged.Broadcast(CurrentHealth);
OnHealthDepleted.Broadcast();
}

bool UHealthComponent::IsAlive() const
{
return CurrentHealth > 0.0f;
}
