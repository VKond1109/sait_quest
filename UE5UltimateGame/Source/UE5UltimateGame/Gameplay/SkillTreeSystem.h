#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USkillTreeSystem.generated.h"

// Тип навыка
UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Passive,      // Пассивный
    Active,       // Активный
    Ultimate,     // Ультимативный
    Trait         // Особенность
};

// Категория навыка
UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
    Combat,
    Defense,
    Utility,
    Movement,
    Crafting,
    Social,
    Special
};

// Данные навыка
USTRUCT(BlueprintType)
struct FSkillData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString IconTexturePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
    int32 LevelRequirement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
    TArray<FString> PrerequisiteSkills; // Требуемые навыки

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
    int32 SkillPointsCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
    int32 CurrencyCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    float Cooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    int32 DamageBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    int32 DefenseBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    float SpeedBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    int32 MaxLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    bool bIsUnlocked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    int32 CurrentLevel;

    FSkillData() 
        : Type(ESkillType::Passive), Category(ESkillCategory::Combat)
        , LevelRequirement(1), SkillPointsCost(1), CurrencyCost(0)
        , Duration(0.f), Cooldown(0.f), DamageBonus(0), DefenseBonus(0)
        , SpeedBonus(0.f), MaxLevel(5), bIsUnlocked(false), CurrentLevel(0) {}
};

// Узел дерева навыков
USTRUCT(BlueprintType)
struct FSkillNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    FSkillData Skill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    FVector2D Position; // Позиция в UI

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connections")
    TArray<FString> ConnectedNodes; // ID связанных узлов

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    bool bIsAvailable; // Доступно для изучения

    FSkillNode() : Position(FVector2D::ZeroVector), bIsAvailable(false) {}
};

UCLASS()
class UE5ULTIMATEGAME_API USkillTreeSystem : public UObject
{
    GENERATED_BODY()

public:
    USkillTreeSystem();

    // === ИНИЦИАЛИЗАЦИЯ ===
    void Initialize();
    void LoadSkillTree(const FString& TreeID);

    // === УПРАВЛЕНИЕ НАВЫКАМИ ===
    UFUNCTION(BlueprintCallable, Category = "Skills")
    bool UnlockSkill(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skills")
    bool UpgradeSkill(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skills")
    bool CanUnlockSkill(const FString& SkillID) const;

    UFUNCTION(BlueprintCallable, Category = "Skills")
    bool CanUpgradeSkill(const FString& SkillID) const;

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void ResetSkill(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void ResetAllSkills();

    // === ПОЛУЧЕНИЕ ДАННЫХ ===
    UFUNCTION(BlueprintCallable, Category = "Skills")
    FSkillData GetSkill(const FString& SkillID) const;

    UFUNCTION(BlueprintCallable, Category = "Skills")
    TArray<FSkillData> GetAllSkills() const;

    UFUNCTION(BlueprintCallable, Category = "Skills")
    TArray<FSkillData> GetUnlockedSkills() const;

    UFUNCTION(BlueprintCallable, Category = "Skills")
    TArray<FSkillData> GetAvailableSkills() const;

    UFUNCTION(BlueprintCallable, Category = "Skills")
    TArray<FSkillData> GetSkillsByCategory(ESkillCategory Category) const;

    // === ДЕРЕВО ===
    UFUNCTION(BlueprintCallable, Category = "Tree")
    TArray<FSkillNode> GetSkillTree() const { return SkillTree; }

    UFUNCTION(BlueprintCallable, Category = "Tree")
    FSkillNode GetSkillNode(const FString& SkillID) const;

    UFUNCTION(BlueprintCallable, Category = "Tree")
    void UpdateSkillTreeAvailability();

    // === ОЧКИ НАВЫКОВ ===
    UFUNCTION(BlueprintCallable, Category = "Points")
    int32 GetAvailableSkillPoints() const { return AvailableSkillPoints; }

    UFUNCTION(BlueprintCallable, Category = "Points")
    int32 GetTotalSkillPoints() const { return TotalSkillPoints; }

    UFUNCTION(BlueprintCallable, Category = "Points")
    void AddSkillPoints(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Points")
    void SpendSkillPoints(int32 Amount);

    // === ПРОГРЕССИЯ ===
    UFUNCTION(BlueprintCallable, Category = "Progression")
    int32 CalculateTotalPower() const;

    UFUNCTION(BlueprintCallable, Category = "Progression")
    FString GetBuildName() const;

    // === СОХРАНЕНИЕ/ЗАГРУЗКА ===
    UFUNCTION(BlueprintCallable, Category = "SaveLoad")
    FString SerializeToString() const;

    UFUNCTION(BlueprintCallable, Category = "SaveLoad")
    bool DeserializeFromString(const FString& Data);

    // События
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnSkillUnlocked(const FSkillData& Skill);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnSkillUpgraded(const FSkillData& Skill, int32 NewLevel);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnSkillReset(const FString& SkillID);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnSkillPointsChanged(int32 Current, int32 Total);

private:
    TArray<FSkillNode> SkillTree;
    TMap<FString, FSkillData> SkillsMap;
    
    int32 AvailableSkillPoints;
    int32 TotalSkillPoints;
    int32 PlayerLevel;

    bool HasPrerequisites(const FSkillData& Skill) const;
    void RecalculateAvailability();
};
