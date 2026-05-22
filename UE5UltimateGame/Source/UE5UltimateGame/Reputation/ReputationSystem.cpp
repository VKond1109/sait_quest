#include "ReputationSystem.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

UReputationSystemComponent::UReputationSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    Notoriety = 0.0f;

    // Установка порогов репутации
    RelationThresholds.Add(EFactionRelation::Enemy, FIntPoint(-1000, -500));
    RelationThresholds.Add(EFactionRelation::Hostile, FIntPoint(-499, -200));
    RelationThresholds.Add(EFactionRelation::Unfriendly, FIntPoint(-199, -50));
    RelationThresholds.Add(EFactionRelation::Neutral, FIntPoint(-49, 49));
    RelationThresholds.Add(EFactionRelation::Friendly, FIntPoint(50, 499));
    RelationThresholds.Add(EFactionRelation::Allied, FIntPoint(500, 1000));
}

void UReputationSystemComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Инициализация стартовых фракций может быть выполнена здесь или через Blueprint
}

void UReputationSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UReputationSystemComponent::InitializeFaction(const FString& FactionID, const FString& FactionName, int32 StartReputation)
{
    if (FactionRelations.Contains(FactionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Faction %s already initialized"), *FactionID);
        return;
    }

    FFactionData NewFaction;
    NewFaction.FactionID = FactionID;
    NewFaction.FactionName = FactionName;
    NewFaction.ReputationPoints = StartReputation;
    NewFaction.CurrentRelation = CalculateRelation(StartReputation);

    FactionRelations.Add(FactionID, NewFaction);

    UE_LOG(LogTemp, Log, TEXT("Initialized faction: %s with reputation %d"), *FactionName, StartReputation);
}

void UReputationSystemComponent::AddReputation(const FString& FactionID, int32 Amount)
{
    if (!FactionRelations.Contains(FactionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Faction %s not found"), *FactionID);
        return;
    }

    FFactionData& Faction = FactionRelations[FactionID];
    int32 OldReputation = Faction.ReputationPoints;
    EFactionRelation OldRelation = Faction.CurrentRelation;

    Faction.ReputationPoints = FMath::Clamp(Faction.ReputationPoints + Amount, -1000, 1000);
    Faction.CurrentRelation = CalculateRelation(Faction.ReputationPoints);

    // Проверка изменения отношения
    if (OldRelation != Faction.CurrentRelation)
    {
        OnRelationChanged(FactionID, OldRelation, Faction.CurrentRelation);
        UE_LOG(LogTemp, Log, TEXT("Relation with %s changed from %d to %d"), 
               *FactionName, static_cast<int32>(OldRelation), static_cast<int32>(Faction.CurrentRelation));
    }

    OnReputationChanged(FactionID, Faction.ReputationPoints, Faction.CurrentRelation);
    UpdatePricesForFaction(FactionID);

    UE_LOG(LogTemp, Log, TEXT("Added %d reputation to %s. Total: %d"), Amount, *Faction.FactionName, Faction.ReputationPoints);
}

void UReputationSystemComponent::RemoveReputation(const FString& FactionID, int32 Amount)
{
    AddReputation(FactionID, -Amount);
}

EFactionRelation UReputationSystemComponent::GetRelationWithFaction(const FString& FactionID)
{
    if (!FactionRelations.Contains(FactionID))
    {
        return EFactionRelation::Neutral;
    }

    return FactionRelations[FactionID].CurrentRelation;
}

int32 UReputationSystemComponent::GetReputationPoints(const FString& FactionID)
{
    if (!FactionRelations.Contains(FactionID))
    {
        return 0;
    }

    return FactionRelations[FactionID].ReputationPoints;
}

bool UReputationSystemComponent::HasUnlockedPerk(const FString& FactionID, const FString& PerkID)
{
    if (!FactionRelations.Contains(FactionID))
    {
        return false;
    }

    return FactionRelations[FactionID].UnlockedPerks.Contains(PerkID);
}

void UReputationSystemComponent::UnlockPerk(const FString& FactionID, const FString& PerkID)
{
    if (!FactionRelations.Contains(FactionID))
    {
        return;
    }

    FFactionData& Faction = FactionRelations[FactionID];
    
    if (!Faction.UnlockedPerks.Contains(PerkID))
    {
        Faction.UnlockedPerks.Add(PerkID);
        OnPerkUnlocked(FactionID, PerkID);
        UE_LOG(LogTemp, Log, TEXT("Unlocked perk %s for faction %s"), *PerkID, *Faction.FactionName);
    }
}

void UReputationSystemComponent::SpreadRumors(const FString& Action, float ImpactRadius)
{
    // Влияние на репутацию с несколькими фракциями в радиусе
    UE_LOG(LogTemp, Log, TEXT("Spreading rumors about: %s with radius %f"), *Action, ImpactRadius);

    // Здесь должна быть логика поиска NPC и обновления их отношения
    for (auto& FactionPair : FactionRelations)
    {
        // Небольшое изменение репутации в зависимости от действия
        int32 Impact = FMath::RandRange(-10, 10);
        AddReputation(FactionPair.Key, Impact);
    }
}

void UReputationSystemComponent::CommitCrime(const FString& FactionID, int32 Severity)
{
    if (!FactionRelations.Contains(FactionID))
    {
        return;
    }

    // Уменьшение репутации за преступление
    int32 ReputationLoss = Severity * 10;
    RemoveReputation(FactionID, ReputationLoss);

    // Увеличение известности (Notoriety)
    Notoriety = FMath::Clamp(Notoriety + Severity * 2.0f, 0.0f, 100.0f);

    NotifyNearbyNPCs(FactionID, 5000.0f); // 50 метров

    UE_LOG(LogTemp, Warning, TEXT("Committed crime against %s with severity %d. Lost %d reputation"), 
           *FactionID, Severity, ReputationLoss);
}

void UReputationSystemComponent::PerformGoodDeed(const FString& FactionID, int32 Merit)
{
    if (!FactionRelations.Contains(FactionID))
    {
        return;
    }

    // Увеличение репутации за хорошее дело
    int32 ReputationGain = Merit * 10;
    AddReputation(FactionID, ReputationGain);

    // Небольшое снижение известности
    Notoriety = FMath::Clamp(Notoriety - Merit * 1.0f, 0.0f, 100.0f);

    UE_LOG(LogTemp, Log, TEXT("Performed good deed for %s with merit %d. Gained %d reputation"), 
           *FactionID, Merit, ReputationGain);
}

bool UReputationSystemComponent::HireMercenary(const FString& FactionID, int32 Cost)
{
    if (!FactionRelations.Contains(FactionID))
    {
        return false;
    }

    FFactionData& Faction = FactionRelations[FactionID];

    // Проверка достаточного уровня отношений
    if (Faction.CurrentRelation != EFactionRelation::Friendly && 
        Faction.CurrentRelation != EFactionRelation::Allied)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot hire mercenary from %s. Relation too low"), *Faction.FactionName);
        return false;
    }

    // Применение модификатора цены
    float PriceModifier = GetPriceModifier(FactionID);
    int32 FinalCost = FMath::CeilToInt(Cost * PriceModifier);

    UE_LOG(LogTemp, Log, TEXT("Hired mercenary from %s for %d (modifier: %f)"), 
           *Faction.FactionName, FinalCost, PriceModifier);
    
    // Здесь должна быть логика списания денег и спавна наемника
    return true;
}

bool UReputationSystemComponent::RequestAssistance(const FString& FactionID)
{
    if (!FactionRelations.Contains(FactionID))
    {
        return false;
    }

    FFactionData& Faction = FactionRelations[FactionID];

    // Проверка достаточного уровня отношений
    if (Faction.CurrentRelation == EFactionRelation::Enemy || 
        Faction.CurrentRelation == EFactionRelation::Hostile)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot request assistance from %s. They are hostile"), *Faction.FactionName);
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Requested assistance from %s"), *Faction.FactionName);
    
    // Здесь должна быть логика вызова помощи
    return true;
}

float UReputationSystemComponent::GetPriceModifier(const FString& FactionID)
{
    if (!FactionRelations.Contains(FactionID))
    {
        return 1.0f;
    }

    FFactionData& Faction = FactionRelations[FactionID];
    
    // Модификатор цены в зависимости от отношений
    switch (Faction.CurrentRelation)
    {
        case EFactionRelation::Allied:
            return 0.7f; // 30% скидка
        case EFactionRelation::Friendly:
            return 0.85f; // 15% скидка
        case EFactionRelation::Neutral:
            return 1.0f; // Обычная цена
        case EFactionRelation::Unfriendly:
            return 1.25f; // 25% наценка
        case EFactionRelation::Hostile:
        case EFactionRelation::Enemy:
            return 2.0f; // Двойная цена или отказ в обслуживании
        default:
            return 1.0f;
    }
}

void UReputationSystemComponent::OnReputationChanged(const FString& FactionID, int32 NewValue, EFactionRelation NewRelation)
{
    // Делегат для UI и других систем
}

void UReputationSystemComponent::OnRelationChanged(const FString& FactionID, EFactionRelation OldRelation, EFactionRelation NewRelation)
{
    // Делегат для UI, квестов и других систем
}

void UReputationSystemComponent::OnPerkUnlocked(const FString& FactionID, const FString& PerkID)
{
    // Делегат для UI и других систем
}

EFactionRelation UReputationSystemComponent::CalculateRelation(int32 ReputationPoints) const
{
    for (auto& ThresholdPair : RelationThresholds)
    {
        if (ReputationPoints >= ThresholdPair.Value.X && ReputationPoints <= ThresholdPair.Value.Y)
        {
            return ThresholdPair.Key;
        }
    }

    return EFactionRelation::Neutral;
}

void UReputationSystemComponent::UpdatePricesForFaction(const FString& FactionID)
{
    // Уведомление торговых систем об изменении цен
    UE_LOG(LogTemp, Log, TEXT("Updated prices for faction %s"), *FactionID);
}

void UReputationSystemComponent::NotifyNearbyNPCs(const FString& FactionID, float Radius)
{
    // Уведомление NPC в радиусе о действиях игрока
    UE_LOG(LogTemp, Log, TEXT("Notifying NPCs of faction %s within radius %f"), *FactionID, Radius);
}
