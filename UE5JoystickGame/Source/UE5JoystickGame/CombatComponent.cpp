// Copyright 2024. All rights reserved.

#include "CombatComponent.h"
#include "Engine/HitResult.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

UCombatComponent::UCombatComponent()
{
PrimaryComponentTick.bCanEverTick = true;
CurrentState = ECombatState::None;
AttackData.Damage = 10.0f;
AttackData.Range = 200.0f;
AttackData.AttackCooldown = 1.0f;
}

void UCombatComponent::BeginPlay()
{
Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::StartAiming()
{
if (CurrentState == ECombatState::None || CurrentState == ECombatState::Cooldown)
{
CurrentState = ECombatState::Aiming;
}
}

void UCombatComponent::StopAiming()
{
if (CurrentState == ECombatState::Aiming)
{
CurrentState = ECombatState::None;
}
}

void UCombatComponent::PerformAttack()
{
if (!CanAttack()) return;

CurrentState = ECombatState::Attacking;

FHitResult HitResult;
if (LineTraceAttack(HitResult))
{
if (HitResult.GetActor())
{
UGameplayStatics::ApplyDamage(
HitResult.GetActor(),
AttackData.Damage,
GetOwner()->GetInstigatorController(),
GetOwner(),
UDamageType::StaticClass()
);
OnDamageDealt(AttackData.Damage, HitResult.GetActor());
}
}

StartCooldown();
}

ECombatState UCombatComponent::GetCombatState() const
{
return CurrentState;
}

bool UCombatComponent::CanAttack() const
{
return CurrentState == ECombatState::None || CurrentState == ECombatState::Aiming;
}

bool UCombatComponent::LineTraceAttack(FHitResult& OutHit)
{
if (!GetOwner()) return false;

AActor* Owner = GetOwner();
FVector StartTrace = Owner->GetActorLocation();

FVector ForwardVector = Owner->GetActorForwardVector();
if (CurrentState == ECombatState::Aiming)
{
AController* Controller = Owner->GetInstigatorController();
if (Controller)
{
FVector ViewLocation;
FRotator ViewRotation;
Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
StartTrace = ViewLocation;
ForwardVector = ViewRotation.Vector();
}
}

FVector EndTrace = StartTrace + (ForwardVector * AttackData.Range);

FCollisionQueryParams QueryParams;
QueryParams.AddIgnoredActor(Owner);

return GetWorld()->LineTraceSingleByChannel(
OutHit,
StartTrace,
EndTrace,
ECC_Visibility,
QueryParams
);
}

void UCombatComponent::StartCooldown()
{
CurrentState = ECombatState::Cooldown;

GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, FTimerDelegate::CreateLambda([this]()
{
CurrentState = ECombatState::None;
}), AttackData.AttackCooldown, false);
}
