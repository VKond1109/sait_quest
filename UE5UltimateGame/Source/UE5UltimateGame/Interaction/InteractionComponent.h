#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableInterface.h"
#include "UInteractionComponent.generated.h"

// Структура для найденного объекта взаимодействия
USTRUCT(BlueprintType)
struct FInteractionTarget
{
    GENERATED_BODY()

    UPROPERTY()
    AActor* Actor;

    UPROPERTY()
    TScriptInterface<IInteractableInterface> Interactable;

    UPROPERTY()
    float Distance;

    FInteractionTarget() : Actor(nullptr), Distance(0.f) {}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ULTIMATEGAME_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractionComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // Настройки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float InteractionRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float InteractionCheckInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

    // Текущий целевой объект
    UPROPERTY(BlueprintReadOnly, Category = "Status")
    AActor* CurrentTarget;

    UPROPERTY(BlueprintReadOnly, Category = "Status")
    FString CurrentInteractionText;

    UPROPERTY(BlueprintReadOnly, Category = "Status")
    bool bCanInteract;

    // События
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void PerformInteraction();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void UpdateInteractionTargets();

    // Делегаты для UI
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionChanged, AActor*, Target, bool, bCanInteract);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionTextUpdated, FString, Text);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnInteractionChanged OnInteractionStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnInteractionTextUpdated OnInteractionTextChanged;

private:
    float TimeSinceLastCheck;
    TArray<FInteractionTarget> FoundTargets;

    void SortTargetsByDistance();
};
