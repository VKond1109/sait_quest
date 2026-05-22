#include "EcosystemComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UEcosystemComponent::UEcosystemComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    CreatureType = ECreatureType::Herbivore;
    Health = 100.0f;
    Hunger = 0.0f;
    MaxSpeed = 500.0f;
    CurrentState = ECreatureState::Idle;
    CurrentTarget = nullptr;
    TimeSinceLastUpdate = 0.0f;
    WanderTimer = 0.0f;
}

void UEcosystemComponent::BeginPlay()
{
    Super::BeginPlay();
    
    LastKnownSafeLocation = GetOwner()->GetActorLocation();
}

void UEcosystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    TimeSinceLastUpdate += DeltaTime;
    
    // Обновление поведения каждые 0.5 секунды
    if (TimeSinceLastUpdate >= 0.5f)
    {
        UpdateBehavior(TimeSinceLastUpdate);
        TimeSinceLastUpdate = 0.0f;
    }
    
    // Увеличение голода со временем
    Hunger += DeltaTime * 2.0f; // 2 единицы в секунду
    if (Hunger >= 100.0f)
    {
        Hunger = 100.0f;
        // Получение урона от голода
        Health -= DeltaTime * 5.0f;
        if (Health <= 0.0f)
        {
            OnDeath();
        }
    }
}

void UEcosystemComponent::UpdateBehavior(float DeltaTime)
{
    if (Health <= 0.0f)
    {
        CurrentState = ECreatureState::Idle;
        return;
    }

    // Приоритеты поведения
    if (Hunger > 70.0f && CurrentState != ECreatureState::Fleeing)
    {
        FindFood();
    }
    else if (CurrentTarget != nullptr && CurrentState == ECreatureState::Hunting)
    {
        // Продолжение охоты
        if (!CurrentTarget->IsValidLowLevel() || CurrentTarget->WasRecentlyRendered(5.0f))
        {
            CurrentState = ECreatureState::Wandering;
            CurrentTarget = nullptr;
        }
    }
    else if (WanderTimer <= 0.0f)
    {
        // Случайное блуждание
        CurrentState = ECreatureState::Wandering;
        WanderTimer = FMath::FRandRange(5.0f, 15.0f);
        
        // Выбор случайного направления
        FVector RandomDir = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
        LastKnownSafeLocation = GetOwner()->GetActorLocation() + RandomDir * 1000.0f;
    }
    else
    {
        WanderTimer -= DeltaTime;
    }
}

void UEcosystemComponent::FindFood()
{
    if (CreatureType == ECreatureType::Carnivore || CreatureType == ECreatureType::Predator)
    {
        // Поиск добычи
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
        
        float ClosestDistance = 2000.0f;
        AActor* ClosestPrey = nullptr;
        
        for (AActor* Actor : FoundActors)
        {
            if (Actor == GetOwner()) continue;
            
            UEcosystemComponent* OtherEco = Actor->FindComponentByClass<UEcosystemComponent>();
            if (OtherEco && (OtherEco->CreatureType == ECreatureType::Herbivore || 
                             OtherEco->CreatureType == ECreatureType::Omnivore))
            {
                float Dist = FVector::Dist(GetOwner()->GetActorLocation(), Actor->GetActorLocation());
                if (Dist < ClosestDistance)
                {
                    ClosestDistance = Dist;
                    ClosestPrey = Actor;
                }
            }
        }
        
        if (ClosestPrey)
        {
            HuntTarget(ClosestPrey);
        }
        else
        {
            CurrentState = ECreatureState::Wandering;
        }
    }
    else
    {
        // Травоядные просто ищут безопасные места
        CurrentState = ECreatureState::Wandering;
    }
}

void UEcosystemComponent::FleeFrom(AActor* Threat)
{
    if (!Threat) return;
    
    CurrentState = ECreatureState::Fleeing;
    CurrentTarget = Threat;
    
    // Вектор направления от угрозы
    FVector Direction = (GetOwner()->GetActorLocation() - Threat->GetActorLocation()).GetSafeNormal();
    LastKnownSafeLocation = GetOwner()->GetActorLocation() + Direction * 1500.0f;
    
    // Таймер для возврата к нормальному поведению
    WanderTimer = 10.0f;
}

void UEcosystemComponent::HuntTarget(AActor* Prey)
{
    if (!Prey) return;
    
    CurrentState = ECreatureState::Hunting;
    CurrentTarget = Prey;
}

void UEcosystemComponent::OnHealthChanged(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, 100.0f);
    
    if (Health <= 30.0f && CurrentState != ECreatureState::Fleeing)
    {
        // Попытка убежать при низком здоровье
        if (CurrentTarget)
        {
            FleeFrom(CurrentTarget);
        }
    }
}

void UEcosystemComponent::OnDeath()
{
    CurrentState = ECreatureState::Idle;
    CurrentTarget = nullptr;
    
    // Логика смерти: превращение в ресурс для других существ
    UE_LOG(LogTemp, Log, TEXT("Creature died at %s"), *GetOwner()->GetActorLocation().ToString());
    
    // Здесь можно заспавнить ресурсы или вызвать события
}
