// Copyright 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

UCLASS()
class UE5JOYSTICKGAME_API AInteractableObject : public AActor
{
GENERATED_BODY()

public:
AInteractableObject();

protected:
virtual void BeginPlay() override;

public:
// Может ли объект быть использован
UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
bool CanInteract() const;
virtual bool CanInteract_Implementation() const;

// Выполнить взаимодействие
UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
void Interact(AActor* Instigator);
virtual void Interact_Implementation(AActor* Instigator);

// Событие при начале взаимодействия
UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
void OnInteractBegin(AActor* Instigator);

// Событие при завершении взаимодействия
UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
void OnInteractEnd(AActor* Instigator);

// Текстовое описание для UI
UFUNCTION(BlueprintPure, Category = "Interaction")
FText GetInteractionText() const;

protected:
// Можно ли взаимодействовать
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
bool bCanBeInteracted = true;

// Текст подсказки
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings", meta=(MultiLine=true))
FText InteractionText = FText::FromString("Взаимодействовать");

// Время выполнения взаимодействия (0 = мгновенно)
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
float InteractionTime = 0.0f;

// Флаг выполнения взаимодействия
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|State")
bool bIsInteracting = false;
};
