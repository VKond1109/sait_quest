// Copyright 2024. All rights reserved.

#include "InteractionComponent.h"
#include "InteractableObject.h"
#include "Engine/HitResult.h"
#include "DrawDebugHelpers.h"

UInteractionComponent::UInteractionComponent()
{
PrimaryComponentTick.bCanEverTick = true;
InteractionRange = 300.0f;
CurrentTargetObject = nullptr;
}

void UInteractionComponent::BeginPlay()
{
Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

if (!GetOwner()) return;

AActor* Owner = GetOwner();
StartTrace = Owner->GetActorLocation();

FVector ForwardVector = Owner->GetActorForwardVector();
EndTrace = StartTrace + (ForwardVector * InteractionRange);

FHitResult HitResult;
FCollisionQueryParams QueryParams;
QueryParams.AddIgnoredActor(Owner);

bool bHit = GetWorld()->LineTraceSingleByChannel(
HitResult,
StartTrace,
EndTrace,
TraceChannel,
QueryParams
);

if (bHit && HitResult.GetActor())
{
AInteractableObject* Interactable = Cast<AInteractableObject>(HitResult.GetActor());
if (Interactable)
{
CurrentTargetObject = Interactable;
return;
}
}

CurrentTargetObject = nullptr;
}

void UInteractionComponent::StartInteraction()
{
if (CurrentTargetObject && CurrentTargetObject->CanInteract())
{
CurrentTargetObject->Interact(GetOwner());
}
}

bool UInteractionComponent::CanInteract() const
{
return CurrentTargetObject != nullptr;
}

AInteractableObject* UInteractionComponent::GetTargetObject() const
{
return CurrentTargetObject;
}
