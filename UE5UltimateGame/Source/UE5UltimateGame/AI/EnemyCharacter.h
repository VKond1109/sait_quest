#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.generated.h"

// Состояния ИИ
UENUM(BlueprintType)
enum class EAIState : uint8
{
    Idle,
    Patrol,
    Chase,
    Attack,
    Search,
    Flee,
    Dead
};

// Типы поведения
UENUM(BlueprintType)
enum class EAIBehaviorType : uint8
{
    Melee,      // Ближний бой
    Ranged,     // Дальний бой
    Support,    // Поддержка
    Tank,       // Танк
    Berserker   // Берсерк
};

UCLASS()
class UE5ULTIMATEGAME_API AEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // === ИИ ЛОГИКА ===
    void UpdateAI(float DeltaTime);
    void ChangeState(EAIState NewState);
    
    // Патрулирование
    void FindNextPatrolPoint();
    void MoveToPatrolPoint();
    
    // Преследование
    void StartChasing(AActor* Target);
    void ChaseTarget();
    
    // Атака
    void PerformAttack();
    void StopAttack();
    
    // Поиск
    void SearchLastKnownPosition();
    
    // Бегство
    void FleeFromTarget();

    // === БОЕВАЯ СИСТЕМА ===
    void TakeDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);
    void Die();
    
    // === НАВЫКИ ===
    void UseSpecialAbility();

public:
    // Настройки ИИ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior")
    EAIBehaviorType BehaviorType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Detection")
    float DetectionRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Detection")
    float LossOfSightRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
    TArray<AActor*> PatrolPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
    float PatrolWaitTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
    float AttackCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
    float DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stats")
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stats")
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stats")
    int32 ExperienceReward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Stats")
    int32 Level;

    // Текущее состояние
    UPROPERTY(BlueprintReadOnly, Category = "AI|Status")
    EAIState CurrentState;

    UPROPERTY(BlueprintReadOnly, Category = "AI|Status")
    AActor* CurrentTarget;

    UPROPERTY(BlueprintReadOnly, Category = "AI|Status")
    FVector LastKnownPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AI|Status")
    bool bIsDead;

    // Компоненты
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UAIPerceptionComponent* PerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UAIBlackboardComponent* BlackboardComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBehaviorTreeComponent* BehaviorTreeComp;

    // События для Blueprint
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnStateChanged(EAIState OldState, EAIState NewState);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnTargetSpotted(AActor* Target);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnDeathEvent();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
    bool IsAlive() const { return !bIsDead; }
};
