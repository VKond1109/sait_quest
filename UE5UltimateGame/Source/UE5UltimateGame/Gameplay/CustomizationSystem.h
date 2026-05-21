#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UCustomizationSystem.generated.h"

// Тип кастомизации
UENUM(BlueprintType)
enum class ECustomizationType : uint8
{
    Head,
    Face,
    Hair,
    Body,
    Arms,
    Legs,
    Feet,
    Hands,
    Accessory,
    Skin,
    Tattoo,
    Paint,
    Decal
};

// Данные части кастомизации
USTRUCT(BlueprintType)
struct FCustomizationPart
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    FString PartID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    FString PartName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    ECustomizationType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    class USkeletalMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    TArray<class UMaterialInterface*> Materials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    bool bIsLocked; // Заблокировано по умолчанию

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    int32 UnlockLevel; // Уровень для разблокировки

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
    int32 Price; // Цена в валюте

    FCustomizationPart() 
        : Type(ECustomizationType::Body), bIsLocked(false), UnlockLevel(1), Price(0) {}
};

// Данные цвета
USTRUCT(BlueprintType)
struct FColorPreset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FString PresetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FLinearColor PrimaryColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FLinearColor SecondaryColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FLinearColor TertiaryColor;

    FColorPreset() : PrimaryColor(FLinearColor::White), SecondaryColor(FLinearColor::Black), TertiaryColor(FLinearColor::Gray) {}
};

UCLASS()
class UE5ULTIMATEGAME_API ACustomizationStation : public AActor
{
    GENERATED_BODY()

public:
    ACustomizationStation();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // === ПРИМЕНЕНИЕ ===
    void ApplyCustomization();
    void PreviewCustomization();
    void ResetToDefault();
    void SaveCustomization();
    void LoadCustomization();

    // === УПРАВЛЕНИЕ ===
    void EquipPart(const FCustomizationPart& Part);
    void UnequipPart(ECustomizationType Type);
    void ChangeColor(ECustomizationType Type, const FLinearColor& NewColor);
    void ApplyPreset(const FString& PresetName);

public:
    // Доступные части
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parts")
    TArray<FCustomizationPart> AvailableParts;

    // Текущая конфигурация
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
    TMap<ECustomizationType, FCustomizationPart> EquippedParts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    TMap<ECustomizationType, FLinearColor> PartColors;

    // Пресеты цветов
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presets")
    TArray<FColorPreset> ColorPresets;

    // Компоненты персонажа для предпросмотра
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USkeletalMeshComponent* PreviewMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCameraComponent* PreviewCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class ULightComponent* PreviewLight;

    // Настройки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bAllowSave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bRequirePayment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float RotationSpeed; // Скорость вращения превью

    // Текущий игрок
    UPROPERTY()
    class AUltimateCharacter* CurrentPlayer;

    // События
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnPartEquipped(ECustomizationType Type, const FCustomizationPart& Part);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnColorChanged(ECustomizationType Type, const FLinearColor& Color);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnPresetApplied(const FString& PresetName);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnCustomizationSaved();

    UFUNCTION(BlueprintCallable, Category = "Preview")
    void RotatePreview(float DeltaRotation);

    UFUNCTION(BlueprintCallable, Category = "Preview")
    void ZoomPreview(float DeltaZoom);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Parts")
    TArray<FCustomizationPart> GetAvailablePartsForType(ECustomizationType Type) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Parts")
    FCustomizationPart GetCurrentPart(ECustomizationType Type) const;
};
