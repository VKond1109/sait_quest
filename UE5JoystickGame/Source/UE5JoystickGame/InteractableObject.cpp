// Copyright 2024. All rights reserved.

#include "InteractableObject.h"

AInteractableObject::AInteractableObject()
{
PrimaryActorTick.bCanEverTick = false;
bCanBeInteracted = true;
InteractionText = FText::FromString("Взаимодействовать");
InteractionTime = 0.0f;
bIsInteracting = false;
}

void AInteractableObject::BeginPlay()
{
Super::BeginPlay();
}

bool AInteractableObject::CanInteract_Implementation() const
{
return bCanBeInteracted && !bIsInteracting;
}

void AInteractableObject::Interact_Implementation(AActor* Instigator)
{
if (!CanInteract()) return;

bIsInteracting = true;
OnInteractBegin(Instigator);

if (InteractionTime > 0.0f)
{
FTimerHandle TimerHandle;
GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, Instigator]()
{
bIsInteracting = false;
OnInteractEnd(Instigator);
}), InteractionTime, false);
}
else
{
bIsInteracting = false;
OnInteractEnd(Instigator);
}
}

FText AInteractableObject::GetInteractionText() const
{
return InteractionText;
}
