#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorSystem.generated.h"

// Типы брони
UENUM(BlueprintType)
enum class EArmorType : uint8
{
    Light UMETA(DisplayName = "Легкая"),
    Medium UMETA(DisplayName = "Средняя"),
    Heavy UMETA(DisplayName = "Тяжелая"),
    Shield UMETA(DisplayName = "Щит")
};

// Слоты экипировки
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    Head UMETA(DisplayName = "Голова"),
    Chest UMETA(DisplayName = "Тело"),
    Legs UMETA(DisplayName = "Ноги"),
    Hands UMETA(DisplayName = "Руки"),
    Feet UMETA(DisplayName = "Ступни"),
    MainHand UMETA(DisplayName = "Основная рука"),
    OffHand UMETA(DisplayName = "Вторая рука"),
    Back UMETA(DisplayName = "Спина"),
    Accessory UMETA(DisplayName = "Аксессуар")
};

/**
 * Структура данных для предмета экипировки
 */
USTRUCT(BlueprintType)
struct FEquipmentItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FString ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    EEquipmentSlot Slot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    EArmorType ArmorType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    float ArmorValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    float Durability = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    float MaxDurability = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    float Weight = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    TMap<FString, float> StatBonuses; // Например: {"Strength", 5.0f}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    UTexture2D* Icon;

    FEquipmentItem()
    {
        ArmorValue = 0.0f;
        Durability = 100.0f;
        MaxDurability = 100.0f;
        Weight = 0.0f;
    }
};

/**
 * Компонент для управления экипировкой и броней
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ULTIMATEGAME_API UArmorSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UArmorSystemComponent();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Карта экипированных предметов по слотам
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    TMap<EEquipmentSlot, FEquipmentItem> EquippedItems;

    // Общая защита
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    float TotalArmorValue = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    float TotalWeight = 0.0f;

    // Функции экипировки
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool EquipItem(const FEquipmentItem& Item);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool UnequipItem(EEquipmentSlot Slot);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    FEquipmentItem GetEquippedItem(EEquipmentSlot Slot);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    float CalculateDamageReduction(float IncomingDamage, FName DamageType);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void RepairItem(EEquipmentSlot Slot, float RepairAmount);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void DamageItem(EEquipmentSlot Slot, float DamageAmount);

    // События
    UFUNCTION()
    void OnItemEquipped(EEquipmentSlot Slot, const FEquipmentItem& Item);

    UFUNCTION()
    void OnItemUnequipped(EEquipmentSlot Slot);

    UFUNCTION()
    void OnDurabilityChanged(EEquipmentSlot Slot, float NewDurability);

private:
    void RecalculateStats();
    void ApplyVisualChanges(EEquipmentSlot Slot, const FEquipmentItem& Item);
    bool CanEquip(const FEquipmentItem& Item) const;
};
