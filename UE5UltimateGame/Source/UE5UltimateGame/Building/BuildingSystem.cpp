#include "BuildingSystem.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

UBuildingSystemComponent::UBuildingSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bIsInBuildMode = false;
    GridSize = 100.0f; // 1 метр
}

void UBuildingSystemComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UBuildingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsInBuildMode)
    {
        // В режиме строительства можно показывать превью
        // Логика обновления превью реализуется в Blueprint или через виджет
    }
}

void UBuildingSystemComponent::EnterBuildMode()
{
    bIsInBuildMode = true;
    UE_LOG(LogTemp, Log, TEXT("Entered build mode"));
}

void UBuildingSystemComponent::ExitBuildMode()
{
    bIsInBuildMode = false;
    UE_LOG(LogTemp, Log, TEXT("Exited build mode"));
}

void UBuildingSystemComponent::SelectElement(EBuildingElementType Type, EBuildingMaterial Material)
{
    CurrentElement.ElementType = Type;
    CurrentElement.Material = Material;

    // Установка стоимости и требований к ресурсам в зависимости от типа и материала
    switch (Material)
    {
        case EBuildingMaterial::Wood:
            CurrentElement.Cost = 10;
            CurrentElement.RequiredResources.Add("Wood", 5);
            break;
        case EBuildingMaterial::Stone:
            CurrentElement.Cost = 20;
            CurrentElement.RequiredResources.Add("Stone", 5);
            break;
        case EBuildingMaterial::Metal:
            CurrentElement.Cost = 30;
            CurrentElement.RequiredResources.Add("Metal", 5);
            break;
        case EBuildingMaterial::Concrete:
            CurrentElement.Cost = 25;
            CurrentElement.RequiredResources.Add("Concrete", 5);
            break;
        case EBuildingMaterial::Glass:
            CurrentElement.Cost = 15;
            CurrentElement.RequiredResources.Add("Glass", 3);
            break;
    }

    UE_LOG(LogTemp, Log, TEXT("Selected element: %d, Material: %d"), static_cast<int32>(Type), static_cast<int32>(Material));
}

bool UBuildingSystemComponent::TryBuildElement(FVector Location, FRotator Rotation)
{
    if (!bIsInBuildMode)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not in build mode"));
        return false;
    }

    // Привязка к сетке
    SnapToGrid(Location, Rotation);

    // Проверка локации
    if (!ValidateBuildLocation(Location, Rotation))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid build location"));
        return false;
    }

    // Проверка ресурсов
    if (!HasRequiredResources(CurrentElement.RequiredResources))
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough resources"));
        return false;
    }

    // Потребление ресурсов
    ConsumeResources(CurrentElement.RequiredResources);

    // Спавн здания
    AActor* NewBuilding = SpawnBuildingActor(CurrentElement, Location, Rotation);
    
    if (NewBuilding)
    {
        BuiltElements.Add(CurrentElement);
        OnElementBuilt(CurrentElement, Location);
        UE_LOG(LogTemp, Log, TEXT("Built element at %s"), *Location.ToString());
        return true;
    }

    return false;
}

bool UBuildingSystemComponent::DemolishElement(AActor* BuildingActor)
{
    if (!BuildingActor)
    {
        return false;
    }

    // Возврат части ресурсов при сносе
    UE_LOG(LogTemp, Log, TEXT("Demolished building actor"));
    
    BuiltElements.RemoveSingle(CurrentElement);
    OnElementDemolished(CurrentElement);

    BuildingActor->Destroy();
    return true;
}

void UBuildingSystemComponent::RepairElement(AActor* BuildingActor, float RepairAmount)
{
    if (!BuildingActor)
    {
        return;
    }

    // Логика ремонта здания
    // В реальном проекте здесь должно быть изменение переменной Health на акторе здания
    UE_LOG(LogTemp, Log, TEXT("Repaired building for %f"), RepairAmount);
}

bool UBuildingSystemComponent::HasRequiredResources(const TMap<FString, int32>& Resources)
{
    // Проверка наличия ресурсов в инвентаре игрока
    // В реальном проекте здесь должна быть проверка компонента инвентаря
    for (auto& Resource : Resources)
    {
        UE_LOG(LogTemp, Log, TEXT("Checking resource: %s, Required: %d"), *Resource.Key, Resource.Value);
        // Заглушка - всегда возвращает true для демонстрации
    }
    return true;
}

void UBuildingSystemComponent::ConsumeResources(const TMap<FString, int32>& Resources)
{
    // Удаление ресурсов из инвентаря игрока
    for (auto& Resource : Resources)
    {
        UE_LOG(LogTemp, Log, TEXT("Consumed %d of %s"), Resource.Value, *Resource.Key);
    }
}

void UBuildingSystemComponent::BuildWall(FVector Start, FVector End, EBuildingMaterial Material)
{
    FBuildingElement WallElement;
    WallElement.ElementType = EBuildingElementType::Wall;
    WallElement.Material = Material;
    
    float Distance = FVector::Dist(Start, End);
    int32 NumWalls = FMath::CeilToInt(Distance / GridSize);

    for (int32 i = 0; i < NumWalls; i++)
    {
        FVector Location = FMath::Lerp(Start, End, (float)i / NumWalls);
        FRotator Rotation = FRotationMatrix::MakeFromX(End - Start).Rotator();
        
        TryBuildElement(Location, Rotation);
    }

    UE_LOG(LogTemp, Log, TEXT("Built wall from %s to %s with %d elements"), *Start.ToString(), *End.ToString(), NumWalls);
}

void UBuildingSystemComponent::BuildFloor(FVector Center, float Size, EBuildingMaterial Material)
{
    FBuildingElement FloorElement;
    FloorElement.ElementType = EBuildingElementType::Floor;
    FloorElement.Material = Material;

    int32 TilesPerSide = FMath::CeilToInt(Size / GridSize);
    
    for (int32 X = 0; X < TilesPerSide; X++)
    {
        for (int32 Y = 0; Y < TilesPerSide; Y++)
        {
            FVector Location = Center + FVector((X - TilesPerSide/2) * GridSize, (Y - TilesPerSide/2) * GridSize, 0);
            FRotator Rotation = FRotator::ZeroRotator;
            
            TryBuildElement(Location, Rotation);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Built floor at %s with size %f"), *Center.ToString(), Size);
}

void UBuildingSystemComponent::OnElementBuilt(const FBuildingElement& Element, FVector Location)
{
    // Делегат для UI и других систем
}

void UBuildingSystemComponent::OnElementDemolished(const FBuildingElement& Element)
{
    // Делегат для UI и других систем
}

void UBuildingSystemComponent::OnBuildingDamaged(AActor* Building, float Damage)
{
    // Делегат для UI и других систем
}

bool UBuildingSystemComponent::ValidateBuildLocation(FVector Location, FRotator Rotation) const
{
    // Проверка: нет ли перекрытий с другими объектами
    // Проверка: достижимость места строительства
    // Проверка: соответствие правилам размещения (стены должны соединяться и т.д.)
    
    // Заглушка - всегда возвращает true
    return true;
}

void UBuildingSystemComponent::SnapToGrid(FVector& Location, FRotator& Rotation)
{
    // Привязка позиции к сетке
    Location.X = FMath::RoundToFloat(Location.X / GridSize) * GridSize;
    Location.Y = FMath::RoundToFloat(Location.Y / GridSize) * GridSize;
    Location.Z = FMath::RoundToFloat(Location.Z / GridSize) * GridSize;

    // Привязка вращения к 45 градусам
    float Yaw = FMath::RoundToFloat(Rotation.Yaw / 45.0f) * 45.0f;
    Rotation = FRotator(0.0f, Yaw, 0.0f);
}

AActor* UBuildingSystemComponent::SpawnBuildingActor(const FBuildingElement& Element, FVector Location, FRotator Rotation)
{
    // В реальном проекте здесь должен быть спавн префаба здания
    // Например: GetWorld()->SpawnActor<AActor>(BuildingClass, Location, Rotation);
    
    UE_LOG(LogTemp, Log, TEXT("Spawning building actor: Type=%d, Material=%d at %s"), 
           static_cast<int32>(Element.ElementType), 
           static_cast<int32>(Element.Material), 
           *Location.ToString());
    
    // Заглушка - возвращаем nullptr для демонстрации
    return nullptr;
}
