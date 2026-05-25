#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReputationSystem.generated.h"

// Отношения между фракциями
UENUM(BlueprintType)
enum class EFactionRelation : uint8
{
    Allied UMETA(DisplayName = "Союзник"),
    Friendly UMETA(DisplayName = "Друг"),
    Neutral UMETA(DisplayName = "Нейтрал"),
    Unfriendly UMETA(DisplayName = "Недружелюбен"),
    Hostile UMETA(DisplayName = "Враг"),
    Enemy UMETA(DisplayName = "Заклятый враг")
};

/**
 * Структура данных о репутации с фракцией
 */
USTRUCT(BlueprintType)
struct FFactionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation")
    FString FactionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation")
    FString FactionName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reputation")
    int32 ReputationPoints;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reputation")
    EFactionRelation CurrentRelation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation")
    TArray<FString> UnlockedPerks;

    FFactionData()
    {
        ReputationPoints = 0;
        CurrentRelation = EFactionRelation::Neutral;
    }
};

/**
 * Компонент системы репутации и фракций
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ULTIMATEGAME_API UReputationSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UReputationSystemComponent();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Карта всех фракций
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reputation")
    TMap<FString, FFactionData> FactionRelations;

    // Текущая известность игрока
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reputation")
    float Notoriety; // 0-100, влияет на реакцию NPC

    // Функции репутации
    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void InitializeFaction(const FString& FactionID, const FString& FactionName, int32 StartReputation);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void AddReputation(const FString& FactionID, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void RemoveReputation(const FString& FactionID, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    EFactionRelation GetRelationWithFaction(const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    int32 GetReputationPoints(const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    bool HasUnlockedPerk(const FString& FactionID, const FString& PerkID);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void UnlockPerk(const FString& FactionID, const FString& PerkID);

    // Система слухов
    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void SpreadRumors(const FString& Action, float ImpactRadius);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void CommitCrime(const FString& FactionID, int32 Severity);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void PerformGoodDeed(const FString& FactionID, int32 Merit);

    // Наемники и услуги
    UFUNCTION(BlueprintCallable, Category = "Reputation")
    bool HireMercenary(const FString& FactionID, int32 Cost);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    bool RequestAssistance(const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    float GetPriceModifier(const FString& FactionID);

    // События
    UFUNCTION()
    void OnReputationChanged(const FString& FactionID, int32 NewValue, EFactionRelation NewRelation);

    UFUNCTION()
    void OnRelationChanged(const FString& FactionID, EFactionRelation OldRelation, EFactionRelation NewRelation);

    UFUNCTION()
    void OnPerkUnlocked(const FString& FactionID, const FString& PerkID);

private:
    EFactionRelation CalculateRelation(int32 ReputationPoints) const;
    void UpdatePricesForFaction(const FString& FactionID);
    void NotifyNearbyNPCs(const FString& FactionID, float Radius);
    
    // Пороги репутации для разных отношений
    TMap<EFactionRelation, FIntPoint> RelationThresholds;
};
