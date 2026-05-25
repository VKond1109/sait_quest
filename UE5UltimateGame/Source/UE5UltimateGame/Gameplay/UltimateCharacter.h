#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "UUltimateCharacter.h"
#include "InteractableInterface.h"
#include "NetworkSessionManager.h"
#include "UUltimateCharacter.generated.h"

// Состояния персонажа
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    Idle,
    Walking,
    Running,
    Sprinting,
    Crouching,
    Jumping,
    Falling,
    Climbing,
    Swimming,
    Combat,
    Interacting,
    Stunned,
    Dead
};

// Структура характеристик
USTRUCT(BlueprintType)
struct FCharacterStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxStamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SprintSpeedMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Defense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Experience;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 ExperienceToNextLevel;

    FCharacterStats() 
        : Health(100.f), MaxHealth(100.f)
        , Stamina(100.f), MaxStamina(100.f)
        , MovementSpeed(600.f), SprintSpeedMultiplier(1.5f)
        , Damage(10.f), Defense(0.f)
        , Level(1), Experience(0), ExperienceToNextLevel(100) {}
};

UCLASS(config=Game)
class UE5ULTIMATEGAME_API AUltimateCharacter : public ACharacter, public IInteractableInterface
{
    GENERATED_BODY()

public:
    AUltimateCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // === ДВИЖЕНИЕ ===
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Sprint(bool bIsSprinting);
    void Crouch(bool bIsCrouching);
    void JumpAction();
    
    // === БОЕВАЯ СИСТЕМА ===
    void Attack();
    void Aim(bool bIsAiming);
    void UseSkill(int32 SkillIndex);
    void TakeDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);
    void Die();

    // === ВЗАИМОДЕЙСТВИЕ ===
    virtual void Interact_Implementation() override;
    void CheckInteraction();

    // === ПРОГРЕССИЯ ===
    void AddExperience(int32 Amount);
    void LevelUp();
    void UpdateStats();

    // === СЕТЬ ===
    UFUNCTION(Server, Reliable)
    void ServerMove(const FVector_NetQuantize& Location, const FRotator& Rotation);
    
    UFUNCTION(Server, Reliable)
    void ServerAttack();

    UFUNCTION(Client, Reliable)
    void ClientOnDamageTaken(float DamageAmount, float NewHealth);

    UFUNCTION(Client, Reliable)
    void ClientOnDied();

public:
    // Характеристики
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stats", ReplicatedUsing=OnRep_Health)
    FCharacterStats Stats;

    UPROPERTY(BlueprintReadOnly, Category = "Character|State")
    ECharacterState CurrentState;

    UPROPERTY(BlueprintReadWrite, Category = "Character|Combat")
    bool bIsAiming;

    UPROPERTY(BlueprintReadWrite, Category = "Character|Combat")
    bool bIsAttacking;

    // Компоненты
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UJoystickInputComponent* JoystickComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UInteractionComponent* InteractionComponent;

    // Репликация свойств
    UFUNCTION()
    void OnRep_Health();

    // События для Blueprint
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnHealthChanged(float OldHealth, float NewHealth);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnLevelUp(int32 NewLevel);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnDeath();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnAttackStarted();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
    bool IsAlive() const { return Stats.Health > 0.f; }
};
