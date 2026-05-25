// Copyright 2024. All rights reserved.

#include "BaseEnemyAI.h"
#include "HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

ABaseEnemyAI::ABaseEnemyAI()
{
PrimaryActorTick.bCanEverTick = true;
CurrentState = EEnemyState::Idle;
MaxHealth = 100.0f;
PatrolSpeed = 100.0f;
ChaseSpeed = 300.0f;
DetectionRange = 1000.0f;
AttackRange = 150.0f;
AttackDamage = 10.0f;
AttackCooldown = 2.0f;
CurrentPatrolIndex = 0;
TargetActor = nullptr;

HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseEnemyAI::BeginPlay()
{
Super::BeginPlay();

if (HealthComponent)
{
HealthComponent->OnHealthDepleted.AddDynamic(this, &ABaseEnemyAI::TakeDamage);
HealthComponent->SetMaxHealth(MaxHealth);
}

SetEnemyState(EEnemyState::Patrol);
}

void ABaseEnemyAI::Tick(float DeltaTime)
{
Super::Tick(DeltaTime);

if (CurrentState == EEnemyState::Dead) return;

UpdateAI(DeltaTime);
}

EEnemyState ABaseEnemyAI::GetEnemyState() const
{
return CurrentState;
}

void ABaseEnemyAI::TakeDamage(float Damage, AController* EventInstigator, AActor* DamageCauser)
{
if (CurrentState == EEnemyState::Dead || !HealthComponent) return;

HealthComponent->TakeDamage(Damage);

if (HealthComponent->GetCurrentHealth() <= 0.0f)
{
SetEnemyState(EEnemyState::Dead);
OnEnemyDied();
}
else if (CurrentState != EEnemyState::Chase && CurrentState != EEnemyState::Attack)
{
if (DetectPlayer())
{
SetEnemyState(EEnemyState::Chase);
}
}
}

void ABaseEnemyAI::UpdateAI(float DeltaTime)
{
switch (CurrentState)
{
case EEnemyState::Idle:
break;
case EEnemyState::Patrol:
UpdatePatrol(DeltaTime);
break;
case EEnemyState::Chase:
UpdateChase(DeltaTime);
break;
case EEnemyState::Attack:
UpdateAttack(DeltaTime);
break;
case EEnemyState::Dead:
break;
}
}

bool ABaseEnemyAI::DetectPlayer()
{
if (!GetWorld()) return false;

TArray<AActor*> FoundActors;
UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), FoundActors);

for (AActor* Actor : FoundActors)
{
ACharacter* Char = Cast<ACharacter>(Actor);
if (Char && Char->GetController() && !Char->GetController()->IsLocalController())
{
float Distance = FVector::Dist(GetActorLocation(), Char->GetActorLocation());
if (Distance <= DetectionRange)
{
FHitResult HitResult;
FCollisionQueryParams QueryParams;
QueryParams.AddIgnoredActor(this);

if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), Char->GetActorLocation(), ECC_Visibility, QueryParams))
{
if (HitResult.GetActor() == Char)
{
TargetActor = Char;
return true;
}
}
}
}
}

TargetActor = nullptr;
return false;
}

void ABaseEnemyAI::SetEnemyState(EEnemyState NewState)
{
if (CurrentState != NewState)
{
EEnemyState OldState = CurrentState;
CurrentState = NewState;
OnStateChanged(NewState);

if (NewState == EEnemyState::Chase || NewState == EEnemyState::Attack)
{
DetectPlayer();
}
}
}

void ABaseEnemyAI::UpdatePatrol(float DeltaTime)
{
if (PatrolPoints.Num() == 0)
{
SetEnemyState(EEnemyState::Idle);
return;
}

if (DetectPlayer())
{
SetEnemyState(EEnemyState::Chase);
return;
}

AActor* TargetPoint = PatrolPoints[CurrentPatrolIndex];
FVector Location = GetActorLocation();
FVector TargetLocation = TargetPoint->GetActorLocation();
float Distance = FVector::Dist(Location, TargetLocation);

if (Distance < 50.0f)
{
CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
}
else
{
FVector Direction = (TargetLocation - Location).GetSafeNormal();
AddActorWorldOffset(Direction * PatrolSpeed * DeltaTime);
SetActorRotation(UKismetMathLibrary::MakeRotFromX(Direction));
}
}

void ABaseEnemyAI::UpdateChase(float DeltaTime)
{
if (!TargetActor)
{
SetEnemyState(EEnemyState::Patrol);
return;
}

float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());

if (Distance <= AttackRange)
{
SetEnemyState(EEnemyState::Attack);
return;
}

FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
AddActorWorldOffset(Direction * ChaseSpeed * DeltaTime);
SetActorRotation(UKismetMathLibrary::MakeRotFromX(Direction));

if (Distance > DetectionRange)
{
SetEnemyState(EEnemyState::Patrol);
}
}

void ABaseEnemyAI::UpdateAttack(float DeltaTime)
{
if (!TargetActor)
{
SetEnemyState(EEnemyState::Chase);
return;
}

float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());

if (Distance > AttackRange * 1.5f)
{
SetEnemyState(EEnemyState::Chase);
return;
}

FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
SetActorRotation(UKismetMathLibrary::MakeRotFromX(Direction));
}

void ABaseEnemyAI::PerformAttack()
{
if (!TargetActor) return;

UGameplayStatics::ApplyDamage(TargetActor, AttackDamage, GetController(), this, UDamageType::StaticClass());
}
