#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UInventorySystem.generated.h"

// Типы предметов
UENUM(BlueprintType)
enum class EItemType : uint8
{
    None,
    Weapon,
    Armor,
    Consumable,
    Material,
    QuestItem,
    KeyItem,
    Currency,
    SkillBook,
    CraftingIngredient
};

// Редкость предмета
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common,      // Белый
    Uncommon,    // Зеленый
    Rare,        // Синий
    Epic,        // Фиолетовый
    Legendary,   // Оранжевый
    Artifact     // Красный
};

// Структура предмета
USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 StackSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxStackSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float Weight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Value; // Цена

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 LevelRequirement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 DamageBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 DefenseBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 HealthBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 StaminaBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    class UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    class UStaticMesh* Mesh3D;

    FItemData() 
        : ItemType(EItemType::None), Rarity(EItemRarity::Common)
        , StackSize(1), MaxStackSize(99), Weight(0.f), Value(0)
        , LevelRequirement(1), DamageBonus(0), DefenseBonus(0)
        , HealthBonus(0), StaminaBonus(0) {}
};

// Слот инвентаря
USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    FItemData Item;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    int32 Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    bool bIsEquipped;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    int32 SlotIndex;

    FInventorySlot() : Quantity(0), bIsEquipped(false), SlotIndex(-1) {}
};

UCLASS()
class UE5ULTIMATEGAME_API UInventorySystem : public UObject
{
    GENERATED_BODY()

public:
    UInventorySystem();

    // === ИНИЦИАЛИЗАЦИЯ ===
    void Initialize(int32 InMaxSlots, float InMaxWeight);

    // === УПРАВЛЕНИЕ ПРЕДМЕТАМИ ===
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(const FItemData& Item, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FString ItemID, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItemFromSlot(int32 SlotIndex, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ClearInventory();

    // === СЛОТЫ ===
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    FInventorySlot GetSlot(int32 Index) const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    TArray<FInventorySlot> GetAllSlots() const { return Slots; }

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 FindEmptySlot() const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 FindItemSlot(FString ItemID) const;

    // === ПЕРЕМЕЩЕНИЕ ===
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool MoveItem(int32 FromSlot, int32 ToSlot);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool SwapItems(int32 SlotA, int32 SlotB);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool SplitStack(int32 SlotIndex, int32 Amount, int32 TargetSlot);

    // === ЭКИПИРОВКА ===
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool EquipItem(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool UnequipItem(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    FInventorySlot GetEquippedItem(EItemType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    TArray<FInventorySlot> GetAllEquippedItems() const;

    // === СТАТИСТИКА ===
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
    int32 GetCurrentWeight() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
    float GetMaxWeight() const { return MaxWeight; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
    int32 GetUsedSlots() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
    int32 GetMaxSlots() const { return MaxSlots; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
    bool IsFull() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
    bool CanAddItem(const FItemData& Item, int32 Quantity = 1) const;

    // === КРАФТ ===
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool CraftItem(FString RecipeID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
    bool CanCraft(FString RecipeID) const;

    // === СОХРАНЕНИЕ/ЗАГРУЗКА ===
    UFUNCTION(BlueprintCallable, Category = "SaveLoad")
    FString SerializeToString() const;

    UFUNCTION(BlueprintCallable, Category = "SaveLoad")
    bool DeserializeFromString(const FString& Data);

    // События
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnInventoryChanged();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnItemAdded(const FItemData& Item, int32 Quantity);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnItemRemoved(FString ItemID, int32 Quantity);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnItemEquipped(const FItemData& Item);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnWeightChanged(float CurrentWeight, float MaxWeight);

private:
    TArray<FInventorySlot> Slots;
    int32 MaxSlots;
    float MaxWeight;
    
    TArray<FInventorySlot> EquipmentSlots;

    bool CanStack(const FItemData& ItemA, const FItemData& ItemB) const;
    void UpdateWeight();
};
