#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDestructibleObject.generated.h"

// Тип разрушения
UENUM(BlueprintType)
enum class EDestructionType : uint8
{
    None,           // Не разрушается
    Breakable,      // Ломается целиком
    Cuttable,       // Можно разрезать
    Explosive,      // Взрывается
    Collapsible     // Обрушивается
};

UCLASS()
class UE5ULTIMATEGAME_API ADestructibleObject : public AActor
{
    GENERATED_BODY()

public:
    ADestructibleObject();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // === РАЗРУШЕНИЕ ===
    void ApplyDamage(float DamageAmount, FVector ImpactPoint, FVector Impulse);
    void DestroyObject();
    void SpawnDebris();
    void ApplyPhysics();

    // === ВЗАИМОДЕЙСТВИЕ ===
    virtual void Interact_Implementation() override;
    virtual FString GetInteractionText_Implementation() const override;
    virtual bool CanInteract_Implementation(AActor* Interactor) const override;

public:
    // Настройки прочности
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction")
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction")
    EDestructionType DestructionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction")
    bool bIsDestroyed;

    // Физика
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    bool bEnablePhysicsOnDestroy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float ExplosionForce;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float DebrisLifetime;

    // Компоненты
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* MainMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UNiagaraComponent* DestructionVFX;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UAudioComponent* DestructionSFX;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* InteractionComponent;

    // Дебрисы (осколки)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debris")
    TArray<class UStaticMesh*> DebrisMeshes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debris")
    TSubclassOf<class AActor> DebrisActorClass;

    // Награда за разрушение
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
    TArray<FItemData> DropItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
    int32 ExperienceReward;

    // События
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnDamageTaken(float DamageAmount, float RemainingHealth);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnDestroyed();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnDebrisSpawned(TArray<AActor*> DebrisActors);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status")
    float GetHealthPercent() const { return CurrentHealth / MaxHealth; }

    UFUNCTION(BlueprintCallable, Category = "Destruction")
    void ForceDestroy();
};
