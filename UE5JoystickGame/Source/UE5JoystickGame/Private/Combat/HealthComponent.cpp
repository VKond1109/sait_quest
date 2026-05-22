// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat/HealthComponent.h"
#include "AudioComponents/AudioComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	DeathThreshold = 0.0f;
	HealthRegenRate = 0.0f;
	RegenDelay = 5.0f;
	RegenTimer = 0.0f;
	InvincibilityDuration = 0.5f;
	InvincibilityTimer = 0.0f;
	bCanTakeDamage = true;
	bIsAlive = true;

	// Звуковые компоненты (опционально)
	DamageSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DamageSound"));
	DamageSound->bAutoActivate = false;
	DamageSound->SetupAttachment(this);

	DeathSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathSound"));
	DeathSound->bAutoActivate = false;
	DeathSound->SetupAttachment(this);

	// Визуальный эффект урона
	DamageEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DamageEffect"));
	DamageEffect->bAutoActivate = false;
	DamageEffect->SetupAttachment(this);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	bIsAlive = true;
	bCanTakeDamage = true;
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAlive)
		return;

	// Таймер неуязвимости
	if (InvincibilityTimer > 0.0f)
	{
		InvincibilityTimer -= DeltaTime;
		if (InvincibilityTimer <= 0.0f)
		{
			bCanTakeDamage = true;
		}
	}

	// Регенерация здоровья
	if (HealthRegenRate > 0.0f && CurrentHealth < MaxHealth && bCanTakeDamage)
	{
		RegenTimer += DeltaTime;
		if (RegenTimer >= RegenDelay)
		{
			float HealAmount = HealthRegenRate * DeltaTime;
			Heal(HealAmount, nullptr);
		}
	}
}

float UHealthComponent::GetHealthPercent() const
{
	return (CurrentHealth / MaxHealth) * 100.0f;
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::FullHeal()
{
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, 0.0f, nullptr);
}

float UHealthComponent::Heal(float Amount, AActor* Healer)
{
	if (Amount <= 0.0f || !bIsAlive)
		return 0.0f;

	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	float ActualHeal = CurrentHealth - OldHealth;

	if (ActualHeal > 0.0f)
	{
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, ActualHeal, Healer);
		UE_LOG(LogTemp, Log, TEXT("Исцеление: %f, текущее здоровье: %f"), ActualHeal, CurrentHealth);
	}

	return ActualHeal;
}

float UHealthComponent::TakeDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bCanTakeDamage || !bIsAlive || DamageAmount <= 0.0f)
		return 0.0f;

	// Применяем урон
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Max(CurrentHealth - DamageAmount, DeathThreshold);
	float ActualDamage = OldHealth - CurrentHealth;

	// Запускаем таймер неуязвимости
	InvincibilityTimer = InvincibilityDuration;
	bCanTakeDamage = false;
	RegenTimer = 0.0f;

	// Воспроизведение эффектов
	if (DamageSound && DamageSound->SoundCue)
	{
		DamageSound->Play();
	}

	if (DamageEffect && DamageEffect->GetSystemTemplate())
	{
		DamageEffect->Activate(true);
	}

	// Уведомляем об изменении здоровья
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, -ActualDamage, DamageCauser);

	UE_LOG(LogTemp, Log, TEXT("Получен урон: %f, осталось здоровья: %f"), ActualDamage, CurrentHealth);

	// Проверка смерти
	if (CurrentHealth <= DeathThreshold)
	{
		Die(EventInstigator);
	}

	return ActualDamage;
}

void UHealthComponent::Die(AController* InstigatorController)
{
	if (!bIsAlive)
		return;

	bIsAlive = false;
	bCanTakeDamage = false;

	UE_LOG(LogTemp, Log, TEXT("%s умер"), *GetOwner()->GetName());

	// Воспроизведение звука смерти
	if (DeathSound && DeathSound->SoundCue)
	{
		DeathSound->Play();
	}

	// Обработка смерти
	HandleDeath(InstigatorController);

	// Событие смерти
	OnDeath.Broadcast(GetOwner(), InstigatorController);
}

void UHealthComponent::HandleDeath_Implementation(AController* InstigatorController)
{
	// Реализация по умолчанию - может быть переопределена в Blueprint
	// Например: воспроизведение анимации смерти, отключение физики и т.д.
}

bool UHealthComponent::IsAlive() const
{
	return bIsAlive;
}
