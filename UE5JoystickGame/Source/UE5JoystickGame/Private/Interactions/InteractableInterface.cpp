// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactions/InteractableInterface.h"

// Add the function implementations here
void UInteractableInterface::OnLookAtStart_Implementation(AActor* LookingActor)
{
	// Реализация по умолчанию пустая
}

void UInteractableInterface::OnLookAtEnd_Implementation(AActor* LookingActor)
{
	// Реализация по умолчанию пустая
}

void UInteractableInterface::Interact_Implementation(AActor* InteractingActor)
{
	// Реализация по умолчанию пустая
}

FText UInteractableInterface::GetInteractionText_Implementation() const
{
	return FText::FromString(TEXT("Взаимодействовать"));
}

UTexture2D* UInteractableInterface::GetInteractionIcon_Implementation() const
{
	return nullptr;
}
