// Copyright 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
None,
Aiming,
Attacking,
Cooldown
};

USTRUCT(BlueprintType)
struct FAttackData
{
GENERATED_BODY()

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
float Damage = 10.0f;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
float Range = 200.0f;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
float AttackCooldown = 1.0f;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
TSubclassOf<AActor> ProjectileClass;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5JOYSTICKGAME_API UCombatComponent : public UActorComponent
{
GENERATED_BODY()

public:
UCombatComponent();

protected:
virtual void BeginPlay() override;

public:
virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Начать прицеливание
UFUNCTION(BlueprintCallable, Category = "Combat")
void StartAiming();

// Прекратить прицеливание
UFUNCTION(BlueprintCallable, Category = "Combat")
void StopAiming();

// Выполнить атаку
UFUNCTION(BlueprintCallable, Category = "Combat")
void PerformAttack();

// Получить текущее состояние
UFUNCTION(BlueprintPure, Category = "Combat")
ECombatState GetCombatState() const;

// Проверка возможности атаки
UFUNCTION(BlueprintPure, Category = "Combat")
bool CanAttack() const;

// Событие при нанесении урона
UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
void OnDamageDealt(float Damage, AActor* Target);

private:
// Данные атаки
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Settings")
FAttackData AttackData;

// Текущее состояние
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
ECombatState CurrentState;

// Таймер кулдауна
FTimerHandle CooldownTimerHandle;

// Линия трассировки для атаки
bool LineTraceAttack(FHitResult& OutHit);

// Запуск кулдауна
void StartCooldown();
};
