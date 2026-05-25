#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EcosystemComponent.generated.h"

// Типы существ в экосистеме
UENUM(BlueprintType)
enum class ECreatureType : uint8
{
    Herbivore UMETA(DisplayName = "Травоядное"),
    Carnivore UMETA(DisplayName = "Хищник"),
    Omnivore UMETA(DisplayName = "Всеядное"),
    Predator UMETA(DisplayName = "Верховный хищник")
};

// Состояния поведения ИИ экосистемы
UENUM(BlueprintType)
enum class ECreatureState : uint8
{
    Idle UMETA(DisplayName = "Бездействие"),
    Wandering UMETA(DisplayName = "Блуждание"),
    Eating UMETA(DisplayName = "Поедание"),
    Fleeing UMETA(DisplayName = "Бегство"),
    Hunting UMETA(DisplayName = "Охота"),
    Sleeping UMETA(DisplayName = "Сон")
};

/**
 * Компонент для управления живыми существами в экосистеме
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ULTIMATEGAME_API UEcosystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UEcosystemComponent();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Настройки существа
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecosystem")
    ECreatureType CreatureType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecosystem")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecosystem")
    float Hunger = 0.0f; // 0-100

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecosystem")
    float MaxSpeed = 500.0f;

    // Текущее состояние
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ecosystem")
    ECreatureState CurrentState;

    // Ссылки на другие существа
    UPROPERTY()
    AActor* CurrentTarget;

    // Функции поведения
    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void UpdateBehavior(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void FindFood();

    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void FleeFrom(AActor* Threat);

    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void HuntTarget(AActor* Prey);

    // События
    UFUNCTION()
    void OnHealthChanged(float NewHealth);

    UFUNCTION()
    void OnDeath();

private:
    float TimeSinceLastUpdate;
    float WanderTimer;
    FVector LastKnownSafeLocation;
};
