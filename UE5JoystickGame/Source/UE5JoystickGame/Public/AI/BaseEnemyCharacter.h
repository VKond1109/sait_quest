// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnemyCharacter.generated.h"

class UHealthComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;
class UAnimMontage;
class UBehaviorTree;
class UBlackboardData;
class UAudioComponent;

/** Состояния ИИ врага */
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle		UMETA(DisplayName = "Бездействие"),
	Patrol		UMETA(DisplayName = "Патрулирование"),
	Chase		UMETA(DisplayName = "Преследование"),
	Attack		UMETA(DisplayName = "Атака"),
	Dead		UMETA(DisplayName = "Мертв")
};

/**
 * Базовый класс врага с ИИ и боевой системой
 */
UCLASS()
class UE5JOYSTICKGAME_API ABaseEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Компонент здоровья */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	/** Меш врага */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* EnemyMesh;

	/** Звук атаки */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AttackSound;

	/** Звук смерти */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* DeathSound;

	/** Текущее состояние врага */
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	EEnemyState EnemyState;

	/** Поведенческое дерево */
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	/** Blackboard данные */
	UPROPERTY(EditAnywhere, Category = "AI")
	UBlackboardData* BlackboardData;

	/** Дальность обнаружения игрока */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DetectionRange;

	/** Дальность атаки */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackRange;

	/** Урон от атаки */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage;

	/** Скорость передвижения при патрулировании */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PatrolSpeed;

	/** Скорость передвижения при преследовании */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float ChaseSpeed;

	/** Время между атаками */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown;

	/** Таймер атаки */
	float AttackTimer;

	/** Цель врага (игрок) */
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	AActor* Target;

	/** Анимация атаки */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	/** Анимация смерти */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* DeathMontage;

	/** Награда за убийство (очки) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
	int32 ScoreReward;

	/** Выпадающий предмет при смерти */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
	TSubclassOf<AActor> DropItemClass;

	/** Шанс выпадения предмета (0-100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards", meta = (ClampMin = "0", ClampMax = "100"))
	float DropChance;

	/** Атаковать цель */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformAttack();

	/** Нанести урон цели */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DealDamage();

	/** Потеря здоровья */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void OnTakeDamage(float NewHealth, float MaxHealth, float Delta, AActor* DamageCauser);

	/** Смерть врага */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void OnDeath(AActor* Victim, AController* InstigatorController);

	/** Получить текущую скорость */
	UFUNCTION(BlueprintPure, Category = "AI")
	float GetCurrentSpeed() const;

	/** Проверка видимости цели */
	UFUNCTION(BlueprintPure, Category = "AI")
	bool CanSeeTarget() const;

	/** Расстояние до цели */
	UFUNCTION(BlueprintPure, Category = "AI")
	float GetDistanceToTarget() const;
};
