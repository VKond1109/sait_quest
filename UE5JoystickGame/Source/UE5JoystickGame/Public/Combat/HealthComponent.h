// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UAudioComponent;
class UNiagaraComponent;

/** Делегат для уведомления об изменении здоровья */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, float, NewHealth, float, MaxHealth, float, Delta, AActor*, DamageCauser);

/** Делегат для уведомления о смерти */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, AActor*, Victim, AController*, InstigatorController);

/**
 * Компонент здоровья для игрока и врагов
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5JOYSTICKGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	/** Текущее здоровье */
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	/** Максимальное здоровье */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	/** Минимальное здоровье (порог смерти) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DeathThreshold;

	/** Регенерация здоровья в секунду */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthRegenRate;

	/** Задержка перед началом регенерации */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float RegenDelay;

	/** Таймер регенерации */
	float RegenTimer;

	/** Неуязвимость после получения урона */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float InvincibilityDuration;

	/** Таймер неуязвимости */
	float InvincibilityTimer;

	/** Можно ли получить урон сейчас */
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bCanTakeDamage;

	/** Жив ли актер */
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsAlive;

	/** Событие изменения здоровья */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	/** Событие смерти */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

	/** Звуковой компонент получения урона */
	UPROPERTY(VisibleAnywhere, Category = "Health")
	UAudioComponent* DamageSound;

	/** Звуковой компонент смерти */
	UPROPERTY(VisibleAnywhere, Category = "Health")
	UAudioComponent* DeathSound;

	/** Эффект получения урона */
	UPROPERTY(VisibleAnywhere, Category = "Health")
	UNiagaraComponent* DamageEffect;

	/** Получить текущее здоровье в процентах */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	/** Получить текущее здоровье */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth() const;

	/** Получить максимальное здоровье */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const;

	/** Полное исцеление */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void FullHeal();

	/** Исцеление на значение */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float Heal(float Amount, AActor* Healer = nullptr);

	/** Нанесение урона */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageAmount, AController* EventInstigator = nullptr, AActor* DamageCauser = nullptr);

	/** Мгновенная смерть */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die(AController* InstigatorController = nullptr);

	/** Обработка смерти */
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void HandleDeath(AController* InstigatorController);

	/** Проверка жив ли актер */
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsAlive() const;
};
