#include "ArmorSystem.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"

UArmorSystemComponent::UArmorSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    TotalArmorValue = 0.0f;
    TotalWeight = 0.0f;
}

void UArmorSystemComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Инициализация пустых слотов
    for (int32 i = 0; i < static_cast<int32>(EEquipmentSlot::Accessory) + 1; i++)
    {
        EEquipmentSlot Slot = static_cast<EEquipmentSlot>(i);
        EquippedItems.Add(Slot, FEquipmentItem());
    }
}

void UArmorSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UArmorSystemComponent::EquipItem(const FEquipmentItem& Item)
{
    if (!CanEquip(Item))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot equip item: %s"), *Item.ItemName);
        return false;
    }

    // Снятие текущего предмета в этом слоте
    if (EquippedItems.Contains(Item.Slot) && EquippedItems[Item.Slot].ItemID.Len() > 0)
    {
        UnequipItem(Item.Slot);
    }

    // Экипировка нового предмета
    EquippedItems[Item.Slot] = Item;
    
    RecalculateStats();
    ApplyVisualChanges(Item.Slot, Item);
    OnItemEquipped(Item.Slot, Item);

    UE_LOG(LogTemp, Log, TEXT("Equipped item: %s in slot %d"), *Item.ItemName, static_cast<int32>(Item.Slot));
    return true;
}

bool UArmorSystemComponent::UnequipItem(EEquipmentSlot Slot)
{
    if (!EquippedItems.Contains(Slot) || EquippedItems[Slot].ItemID.Len() == 0)
    {
        return false;
    }

    FEquipmentItem OldItem = EquippedItems[Slot];
    EquippedItems[Slot] = FEquipmentItem(); // Пустой предмет
    
    RecalculateStats();
    OnItemUnequipped(Slot);

    UE_LOG(LogTemp, Log, TEXT("Unequipped item from slot %d"), static_cast<int32>(Slot));
    return true;
}

FEquipmentItem UArmorSystemComponent::GetEquippedItem(EEquipmentSlot Slot)
{
    if (EquippedItems.Contains(Slot))
    {
        return EquippedItems[Slot];
    }
    return FEquipmentItem();
}

float UArmorSystemComponent::CalculateDamageReduction(float IncomingDamage, FName DamageType)
{
    float Reduction = 0.0f;
    
    // Базовое снижение от брони
    float ArmorFactor = TotalArmorValue / (TotalArmorValue + 100.0f);
    Reduction = IncomingDamage * ArmorFactor;

    // Модификаторы от типов урона (можно расширить)
    if (DamageType == "Physical")
    {
        Reduction *= 1.0f;
    }
    else if (DamageType == "Fire")
    {
        // Проверка бонусов к огнестойкости
        for (auto& ItemPair : EquippedItems)
        {
            if (ItemPair.Value.StatBonuses.Contains("FireResist"))
            {
                Reduction += ItemPair.Value.StatBonuses["FireResist"] * 0.01f * IncomingDamage;
            }
        }
    }

    return FMath::Clamp(Reduction, 0.0f, IncomingDamage * 0.9f); // Максимум 90% снижения
}

void UArmorSystemComponent::RepairItem(EEquipmentSlot Slot, float RepairAmount)
{
    if (!EquippedItems.Contains(Slot) || EquippedItems[Slot].ItemID.Len() == 0)
    {
        return;
    }

    FEquipmentItem& Item = EquippedItems[Slot];
    Item.Durability = FMath::Clamp(Item.Durability + RepairAmount, 0.0f, Item.MaxDurability);
    
    OnDurabilityChanged(Slot, Item.Durability);
    
    if (Item.Durability >= Item.MaxDurability)
    {
        UE_LOG(LogTemp, Log, TEXT("Item %s fully repaired"), *Item.ItemName);
    }
}

void UArmorSystemComponent::DamageItem(EEquipmentSlot Slot, float DamageAmount)
{
    if (!EquippedItems.Contains(Slot) || EquippedItems[Slot].ItemID.Len() == 0)
    {
        return;
    }

    FEquipmentItem& Item = EquippedItems[Slot];
    Item.Durability = FMath::Clamp(Item.Durability - DamageAmount, 0.0f, Item.MaxDurability);
    
    OnDurabilityChanged(Slot, Item.Durability);

    if (Item.Durability <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Item %s broken!"), *Item.ItemName);
        // Здесь можно добавить логику поломки предмета
    }
}

void UArmorSystemComponent::OnItemEquipped(EEquipmentSlot Slot, const FEquipmentItem& Item)
{
    // Делегат для уведомления UI и других систем
}

void UArmorSystemComponent::OnItemUnequipped(EEquipmentSlot Slot)
{
    // Делегат для уведомления UI и других систем
}

void UArmorSystemComponent::OnDurabilityChanged(EEquipmentSlot Slot, float NewDurability)
{
    // Делегат для обновления UI прочности
}

void UArmorSystemComponent::RecalculateStats()
{
    TotalArmorValue = 0.0f;
    TotalWeight = 0.0f;

    for (auto& ItemPair : EquippedItems)
    {
        if (ItemPair.Value.ItemID.Len() > 0)
        {
            TotalArmorValue += ItemPair.Value.ArmorValue;
            TotalWeight += ItemPair.Value.Weight;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Recalculated stats - Armor: %f, Weight: %f"), TotalArmorValue, TotalWeight);
}

void UArmorSystemComponent::ApplyVisualChanges(EEquipmentSlot Slot, const FEquipmentItem& Item)
{
    // Логика применения визуальных изменений (смена мешей, материалов)
    AActor* Owner = GetOwner();
    if (!Owner) return;

    USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
    if (!Mesh) return;

    // Здесь должна быть логика прикрепления мешей брони к скелету персонажа
    // Например: Mesh->SetSkeletalMesh(Item.ArmorMesh);
    
    UE_LOG(LogTemp, Log, TEXT("Applied visual changes for slot %d"), static_cast<int32>(Slot));
}

bool UArmorSystemComponent::CanEquip(const FEquipmentItem& Item) const
{
    // Проверка требований для экипировки
    // Можно добавить проверки уровня, характеристик и т.д.
    
    if (Item.Slot == EEquipmentSlot::Head || 
        Item.Slot == EEquipmentSlot::Chest || 
        Item.Slot == EEquipmentSlot::Legs ||
        Item.Slot == EEquipmentSlot::Hands ||
        Item.Slot == EEquipmentSlot::Feet ||
        Item.Slot == EEquipmentSlot::MainHand ||
        Item.Slot == EEquipmentSlot::OffHand ||
        Item.Slot == EEquipmentSlot::Back ||
        Item.Slot == EEquipmentSlot::Accessory)
    {
        return true;
    }

    return false;
}
