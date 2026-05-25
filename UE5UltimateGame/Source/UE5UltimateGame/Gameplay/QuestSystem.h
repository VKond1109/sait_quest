#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UQuestSystem.generated.h"

// Тип квеста
UENUM(BlueprintType)
enum class EQuestType : uint8
{
    Main,         // Основной сюжет
    Side,         // Побочный
    Daily,        // Ежедневный
    Weekly,       // Еженедельный
    Event,        // Событие
    Tutorial,     // Обучение
    Hidden        // Скрытый
};

// Состояние квеста
UENUM(BlueprintType)
enum class EQuestState : uint8
{
    NotStarted,
    Available,
    InProgress,
    Completed,
    Failed,
    Abandoned,
    Expired
};

// Тип цели квеста
UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
    Kill,
    Collect,
    Deliver,
    TalkTo,
    GoTo,
    UseItem,
    CraftItem,
    Defend,
    Escort,
    Explore,
    SolvePuzzle
};

// Цель квеста
USTRUCT(BlueprintType)
struct FQuestObjective
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FString ObjectiveID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    EObjectiveType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FString TargetID; // ID цели (NPC, предмет, локация)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    int32 RequiredAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    int32 CurrentAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    bool bIsCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    bool bIsOptional;

    FQuestObjective() 
        : Type(EObjectiveType::Kill), RequiredAmount(1), CurrentAmount(0)
        , bIsCompleted(false), bIsOptional(false) {}
};

// Награда за квест
USTRUCT(BlueprintType)
struct FQuestReward
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    int32 Experience;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    int32 Gold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    int32 SkillPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    TArray<FItemData> Items;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    FString UnlockRecipe; // ID рецепта

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    FString UnlockArea; // ID области

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
    float ReputationGain;

    FQuestReward() : Experience(0), Gold(0), SkillPoints(0), ReputationGain(0.f) {}
};

// Данные квеста
USTRUCT(BlueprintType)
struct FQuestData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString QuestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString LongDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestState State;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    int32 LevelRequirement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString GiverNPC; // Кто дает квест

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString ReceiverNPC; // Кто принимает квест

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<FQuestObjective> Objectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FQuestReward Rewards;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<FString> PrerequisiteQuests; // Требуемые квесты

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<FString> NextQuests; // Следующие квесты

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    float TimeLimit; // Ограничение по времени (0 = нет)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    bool bIsRepeatable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    int32 RepeatCooldownHours;

    FQuestData() 
        : Type(EQuestType::Side), State(EQuestState::NotStarted)
        , LevelRequirement(1), TimeLimit(0.f), bIsRepeatable(false), RepeatCooldownHours(24) {}
};

UCLASS()
class UE5ULTIMATEGAME_API UQuestSystem : public UObject
{
    GENERATED_BODY()

public:
    UQuestSystem();

    // === УПРАВЛЕНИЕ КВЕСТАМИ ===
    UFUNCTION(BlueprintCallable, Category = "Quests")
    bool AcceptQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quests")
    bool AbandonQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quests")
    void CompleteQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quests")
    void FailQuest(const FString& QuestID, const FString& Reason);

    // === ЦЕЛИ ===
    UFUNCTION(BlueprintCallable, Category = "Objectives")
    void UpdateObjective(const FString& QuestID, const FString& ObjectiveID, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Objectives")
    void CompleteObjective(const FString& QuestID, const FString& ObjectiveID);

    UFUNCTION(BlueprintCallable, Category = "Objectives")
    bool CheckObjectiveCompletion(const FString& QuestID, const FString& ObjectiveID) const;

    // === ПОЛУЧЕНИЕ ДАННЫХ ===
    UFUNCTION(BlueprintCallable, Category = "Quests")
    FQuestData GetQuest(const FString& QuestID) const;

    UFUNCTION(BlueprintCallable, Category = "Quests")
    TArray<FQuestData> GetAllQuests() const;

    UFUNCTION(BlueprintCallable, Category = "Quests")
    TArray<FQuestData> GetActiveQuests() const;

    UFUNCTION(BlueprintCallable, Category = "Quests")
    TArray<FQuestData> GetCompletedQuests() const;

    UFUNCTION(BlueprintCallable, Category = "Quests")
    TArray<FQuestData> GetAvailableQuests() const;

    UFUNCTION(BlueprintCallable, Category = "Quests")
    TArray<FQuestData> GetQuestsByType(EQuestType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Quests")
    TArray<FQuestData> GetQuestsFromNPC(const FString& NPC_ID) const;

    // === ПРОВЕРКИ ===
    UFUNCTION(BlueprintCallable, Category = "Checks")
    bool CanAcceptQuest(const FString& QuestID) const;

    UFUNCTION(BlueprintCallable, Category = "Checks")
    bool HasPrerequisites(const FString& QuestID) const;

    UFUNCTION(BlueprintCallable, Category = "Checks")
    bool IsQuestCompleted(const FString& QuestID) const;

    UFUNCTION(BlueprintCallable, Category = "Checks")
    bool IsQuestInProgress(const FString& QuestID) const;

    // === TRACKING ===
    UFUNCTION(BlueprintCallable, Category = "Tracking")
    void TrackQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Tracking")
    void UntrackQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Tracking")
    FString GetTrackedQuestID() const { return TrackedQuestID; }

    // === НАГРАДЫ ===
    UFUNCTION(BlueprintCallable, Category = "Rewards")
    void ClaimRewards(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Rewards")
    FQuestReward GetQuestRewards(const FString& QuestID) const;

    // === СОХРАНЕНИЕ/ЗАГРУЗКА ===
    UFUNCTION(BlueprintCallable, Category = "SaveLoad")
    FString SerializeToString() const;

    UFUNCTION(BlueprintCallable, Category = "SaveLoad")
    bool DeserializeFromString(const FString& Data);

    // События
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnQuestAccepted(const FQuestData& Quest);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnQuestCompleted(const FQuestData& Quest);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnQuestFailed(const FString& QuestID, const FString& Reason);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnObjectiveUpdated(const FString& QuestID, const FString& ObjectiveID, int32 Current, int32 Required);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnQuestTracked(const FString& QuestID);

private:
    TMap<FString, FQuestData> QuestsMap;
    FString TrackedQuestID;
    int32 PlayerLevel;

    void UpdateQuestState(FQuestData& Quest);
    void CheckAllObjectives(FQuestData& Quest);
};
