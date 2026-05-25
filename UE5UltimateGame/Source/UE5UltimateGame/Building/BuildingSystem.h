#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingSystem.generated.h"

// Типы строительных элементов
UENUM(BlueprintType)
enum class EBuildingElementType : uint8
{
    Wall UMETA(DisplayName = "Стена"),
    Floor UMETA(DisplayName = "Пол"),
    Ceiling UMETA(DisplayName = "Потолок"),
    Roof UMETA(DisplayName = "Крыша"),
    Door UMETA(DisplayName = "Дверь"),
    Window UMETA(DisplayName = "Окно"),
    Stairs UMETA(DisplayName = "Лестница"),
    Foundation UMETA(DisplayName = "Фундамент")
};

// Материалы для строительства
UENUM(BlueprintType)
enum class EBuildingMaterial : uint8
{
    Wood UMETA(DisplayName = "Дерево"),
    Stone UMETA(DisplayName = "Камень"),
    Metal UMETA(DisplayName = "Металл"),
    Concrete UMETA(DisplayName = "Бетон"),
    Glass UMETA(DisplayName = "Стекло")
};

/**
 * Структура данных для строительного элемента
 */
USTRUCT(BlueprintType)
struct FBuildingElement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    EBuildingElementType ElementType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    EBuildingMaterial Material;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FString ElementID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    int32 Cost = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    TMap<FString, int32> RequiredResources;

    FBuildingElement()
    {
        Health = 100.0f;
        MaxHealth = 100.0f;
        Cost = 10;
    }
};

/**
 * Компонент системы строительства
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ULTIMATEGAME_API UBuildingSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UBuildingSystemComponent();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Текущий выбранный элемент для строительства
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building")
    FBuildingElement CurrentElement;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building")
    bool bIsInBuildMode;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building")
    TArray<FBuildingElement> BuiltElements;

    // Функции строительства
    UFUNCTION(BlueprintCallable, Category = "Building")
    void EnterBuildMode();

    UFUNCTION(BlueprintCallable, Category = "Building")
    void ExitBuildMode();

    UFUNCTION(BlueprintCallable, Category = "Building")
    void SelectElement(EBuildingElementType Type, EBuildingMaterial Material);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool TryBuildElement(FVector Location, FRotator Rotation);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool DemolishElement(AActor* BuildingActor);

    UFUNCTION(BlueprintCallable, Category = "Building")
    void RepairElement(AActor* BuildingActor, float RepairAmount);

    UFUNCTION(BlueprintCallable, Category = "Building")
    bool HasRequiredResources(const TMap<FString, int32>& Resources);

    UFUNCTION(BlueprintCallable, Category = "Building")
    void ConsumeResources(const TMap<FString, int32>& Resources);

    // Сниппеты для быстрого строительства
    UFUNCTION(BlueprintCallable, Category = "Building")
    void BuildWall(FVector Start, FVector End, EBuildingMaterial Material);

    UFUNCTION(BlueprintCallable, Category = "Building")
    void BuildFloor(FVector Center, float Size, EBuildingMaterial Material);

    // События
    UFUNCTION()
    void OnElementBuilt(const FBuildingElement& Element, FVector Location);

    UFUNCTION()
    void OnElementDemolished(const FBuildingElement& Element);

    UFUNCTION()
    void OnBuildingDamaged(AActor* Building, float Damage);

private:
    bool ValidateBuildLocation(FVector Location, FRotator Rotation) const;
    void SnapToGrid(FVector& Location, FRotator& Rotation);
    AActor* SpawnBuildingActor(const FBuildingElement& Element, FVector Location, FRotator Rotation);
    float GridSize;
};
