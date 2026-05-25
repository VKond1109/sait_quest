// Copyright 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseEnemyAI.generated.h"

class UHealthComponent;
class UAISimpleController;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
Idle,
Patrol,
Chase,
Attack,
Dead
};

UCLASS()
class UE5JOYSTICKGAME_API ABaseEnemyAI : public APawn
{
GENERATED_BODY()

public:
ABaseEnemyAI();

protected:
virtual void BeginPlay() override;

public:
virtual void Tick(float DeltaTime) override;

// Получить состояние врага
UFUNCTION(BlueprintPure, Category = "Enemy")
EEnemyState GetEnemyState() const;

// Нанести урон
UFUNCTION(BlueprintCallable, Category = "Enemy")
void TakeDamage(float Damage, AController* EventInstigator, AActor* DamageCauser);

// Событие смерти
UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
void OnEnemyDied();

// Событие изменения состояния
UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
void OnStateChanged(EEnemyState NewState);

protected:
// Компонент здоровья
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Components")
UHealthComponent* HealthComponent;

// Текущее состояние
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|State")
EEnemyState CurrentState;

// Здоровье
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
float MaxHealth = 100.0f;

// Скорость патрулирования
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
float PatrolSpeed = 100.0f;

// Скорость преследования
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
float ChaseSpeed = 300.0f;

// Дистанция обнаружения игрока
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
float DetectionRange = 1000.0f;

// Дистанция атаки
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
float AttackRange = 150.0f;

// Урон атаки
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
float AttackDamage = 10.0f;

// Время между атаками
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
float AttackCooldown = 2.0f;

// Точки патрулирования
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Patrol")
TArray<AActor*> PatrolPoints;

// Текущая точка патрулирования
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Patrol")
int32 CurrentPatrolIndex = 0;

// Таймер атаки
FTimerHandle AttackTimerHandle;

// Ссылка на цель (игрок)
UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Enemy|State")
AActor* TargetActor;

// Обновление ИИ
virtual void UpdateAI(float DeltaTime);

// Поиск игрока
bool DetectPlayer();

// Переход к состоянию
void SetEnemyState(EEnemyState NewState);

// Логика патрулирования
void UpdatePatrol(float DeltaTime);

// Логика преследования
void UpdateChase(float DeltaTime);

// Логика атаки
void UpdateAttack(float DeltaTime);

// Выполнение атаки
void PerformAttack();
};
