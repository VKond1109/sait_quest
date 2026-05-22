// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTags.h"
#include "EnhancedJoystickPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UAnimationMontage;
class UHealthComponent;
class UVirtualJoystickComponent;

/**
 * Расширенный класс игрока с боевой системой, взаимодействием и продвинутым движением
 */
UCLASS()
class UE5JOYSTICKGAME_API AEnhancedJoystickPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnhancedJoystickPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Камера от 3-го лица */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	/** Компонент камеры */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	/** Компонент здоровья */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComponent;

	/** Контекст маппинга ввода */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	/** Дейтие движения */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** Действие взгляда */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	/** Действие прыжка */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	/** Действие бега */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	/** Дейтие взаимодействия */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	/** Действие атаки */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackAction;

	/** Действие способности */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AbilityAction;

	/** Дейтие смены камеры */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ToggleCameraAction;

	/** Текущий режим камеры (1-е лицо / 3-е лицо) */
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	bool bIsFirstPerson;

	/** Базовая скорость ходьбы */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	/** Скорость бега */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	/** Максимальное количество двойных прыжков */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 MaxDoubleJumps;

	/** Текущее количество доступных двойных прыжков */
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	int32 CurrentDoubleJumps;

	/** Длительность рывка */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashDuration;

	/** Сила рывка */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashForce;

	/** Перезарядка рывка */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashCooldown;

	/** Таймер рывка */
	float DashTimer;

	/** Кулдаун рывка */
	float DashCooldownTimer;

	/** Выполняется ли рывок */
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsDashing;

	/** Взаимодействуемый объект в прицеле */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentInteractable;

	/** Дальность взаимодействия */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange;

	/** Анимация атаки */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	/** Анимация способности */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AbilityMontage;

	/** Урон от атаки */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage;

	/** Перезарядка атаки */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown;

	/** Таймер атаки */
	float AttackTimer;

	/** Можно ли атаковать */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bCanAttack;

	/** Очки игрока */
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 Score;

	/** Событие изменения очков */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnScoreChanged OnScoreChanged;

	/** Движение персонажа */
	void Move(const FInputActionValue& Value);

	/** Вращение камеры */
	void Look(const FInputActionValue& Value);

	/** Прыжок */
	void PerformJump();

	/** Бег */
	void StartSprint();
	void StopSprint();

	/** Взаимодействие */
	void PerformInteract();
	void UpdateInteractable();

	/** Атака */
	void PerformAttack();

	/** Способность (рывок) */
	void PerformAbility();
	void ExecuteDash();

	/** Смена режима камеры */
	void ToggleCameraMode();

	/** Настройка скорости движения */
	void UpdateMovementSpeed();

public:
	/** Получить текущие очки */
	UFUNCTION(BlueprintPure, Category = "Game")
	int32 GetScore() const;

	/** Добавить очки */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void AddScore(int32 Amount);

	/** Получить компонент здоровья */
	UFUNCTION(BlueprintPure, Category = "Health")
	UHealthComponent* GetHealthComponent() const;

	/** Жив ли игрок */
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsAlive() const;
};
