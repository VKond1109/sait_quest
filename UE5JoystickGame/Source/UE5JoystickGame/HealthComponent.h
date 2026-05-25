// Copyright 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepletedSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5JOYSTICKGAME_API UHealthComponent : public UActorComponent
{
GENERATED_BODY()

public:
UHealthComponent();

protected:
virtual void BeginPlay() override;

public:
// Получить текущее здоровье
UFUNCTION(BlueprintPure, Category = "Health")
float GetCurrentHealth() const;

// Получить максимальное здоровье
UFUNCTION(BlueprintPure, Category = "Health")
float GetMaxHealth() const;

// Получить процент здоровья
UFUNCTION(BlueprintPure, Category = "Health")
float GetHealthPercent() const;

// Установить максимальное здоровье
UFUNCTION(BlueprintCallable, Category = "Health")
void SetMaxHealth(float NewMaxHealth);

// Нанести урон
UFUNCTION(BlueprintCallable, Category = "Health")
void TakeDamage(float Damage);

// Восстановить здоровье
UFUNCTION(BlueprintCallable, Category = "Health")
void Heal(float Amount);

// Убить персонажа
UFUNCTION(BlueprintCallable, Category = "Health")
void Kill();

// Проверка жив ли персонаж
UFUNCTION(BlueprintPure, Category = "Health")
bool IsAlive() const;

// Событие изменения здоровья
UPROPERTY(BlueprintAssignable, Category = "Health|Events")
FOnHealthChanged OnHealthChanged;

// Событие смерти
UPROPERTY(BlueprintAssignable, Category = "Health|Events")
FOnHealthDepletedSignature OnHealthDepleted;

protected:
// Текущее здоровье
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health|State")
float CurrentHealth;

// Максимальное здоровье
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Settings")
float MaxHealthValue;

// Флаг бессмертия
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Settings")
bool bIsInvulnerable;
};
