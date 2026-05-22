// Copyright 2024 Ultimate Game Studio. All Rights Reserved.

#include "Combat/UHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	MinHealth = 0.0f;
	bCanRegenerate = true;
	RegenerationDelay = 5.0f;
	RegenerationRate = 2.0f;
	bIsInvulnerable = false;
	
	LastDamageTime = 0.0f;
	bIsRegenerating = false;
	RegenerationTimer = 0.0f;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeComponent();
}

void UHealthComponent::InitializeComponent()
{
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, 0.0f);
}

float UHealthComponent::TakeDamage(float DamageAmount, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsInvulnerable || DamageAmount <= 0.0f || !IsAlive())
	{
		return 0.0f;
	}

	float ActualDamage = FMath::Clamp(DamageAmount, 0.0f, CurrentHealth - MinHealth);
	float OldHealth = CurrentHealth;
	CurrentHealth -= ActualDamage;
	CurrentHealth = FMath::Clamp(CurrentHealth, MinHealth, MaxHealth);

	float HealthDelta = CurrentHealth - OldHealth;
	
	LastDamageTime = GetWorld()->GetTimeSeconds();
	bIsRegenerating = false;
	RegenerationTimer = 0.0f;

	OnHealthChanged.Broadcast(CurrentHealth, HealthDelta);

	// Check for death
	if (CurrentHealth <= MinHealth)
	{
		Kill();
	}

	return ActualDamage;
}

float UHealthComponent::Heal(float HealAmount)
{
	if (!IsAlive() || HealAmount <= 0.0f)
	{
		return 0.0f;
	}

	float OldHealth = CurrentHealth;
	CurrentHealth += HealAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, MinHealth, MaxHealth);

	float HealthDelta = CurrentHealth - OldHealth;

	if (HealthDelta > 0.0f)
	{
		OnHealthChanged.Broadcast(CurrentHealth, HealthDelta);
		
		// Start regeneration if not at max
		if (!IsAtMaxHealth() && bCanRegenerate)
		{
			StartRegeneration();
		}
	}

	return HealthDelta;
}

void UHealthComponent::SetHealth(float NewHealth)
{
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(NewHealth, MinHealth, MaxHealth);
	
	float HealthDelta = CurrentHealth - OldHealth;
	if (FMath::Abs(HealthDelta) > 0.001f)
	{
		OnHealthChanged.Broadcast(CurrentHealth, HealthDelta);
		
		if (CurrentHealth <= MinHealth)
		{
			Kill();
		}
		else if (bCanRegenerate && !IsAtMaxHealth())
		{
			StartRegeneration();
		}
	}
}

void UHealthComponent::Kill()
{
	if (!IsAlive())
	{
		return;
	}

	CurrentHealth = MinHealth;
	OnHealthChanged.Broadcast(CurrentHealth, 0.0f);
	
	// Broadcast death event
	OnDeath.Broadcast(GetOwner());

	// Handle death logic (ragdoll, etc.)
	if (GetOwner())
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			Character->SetLifeSpan(0.0f); // Don't auto-destroy
			// Enable ragdoll here
		}
	}
}

float UHealthComponent::GetHealthPercent() const
{
	if (MaxHealth <= 0.0f)
	{
		return 0.0f;
	}
	return (CurrentHealth / MaxHealth) * 100.0f;
}

bool UHealthComponent::IsAlive() const
{
	return CurrentHealth > MinHealth;
}

bool UHealthComponent::IsAtMaxHealth() const
{
	return FMath::IsNearlyEqual(CurrentHealth, MaxHealth, 0.1f);
}

void UHealthComponent::StartRegeneration()
{
	if (!bCanRegenerate || bIsRegenerating)
	{
		return;
	}

	bIsRegenerating = true;
	RegenerationTimer = 0.0f;
}

void UHealthComponent::HandleRegeneration(float DeltaTime)
{
	if (!bCanRegenerate || !bIsRegenerating || IsAtMaxHealth() || !IsAlive())
	{
		return;
	}

	float TimeSinceDamage = GetWorld()->GetTimeSeconds() - LastDamageTime;
	
	if (TimeSinceDamage >= RegenerationDelay)
	{
		RegenerationTimer += DeltaTime;
		
		if (RegenerationTimer >= 1.0f) // Apply regen every second
		{
			Heal(RegenerationRate);
			RegenerationTimer = 0.0f;
		}
	}
}

// Override Tick to handle regeneration
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	HandleRegeneration(DeltaTime);
}
