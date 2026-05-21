// Copyright 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class AInteractableObject;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5JOYSTICKGAME_API UInteractionComponent : public UActorComponent
{
GENERATED_BODY()

public:
UInteractionComponent();

protected:
virtual void BeginPlay() override;

public:
virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Начать взаимодействие с объектом
UFUNCTION(BlueprintCallable, Category = "Interaction")
void StartInteraction();

// Проверка наличия объекта для взаимодействия
UFUNCTION(BlueprintPure, Category = "Interaction")
bool CanInteract() const;

// Получить объект для взаимодействия
UFUNCTION(BlueprintPure, Category = "Interaction")
AInteractableObject* GetTargetObject() const;

private:
// Дистанция взаимодействия
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
float InteractionRange = 300.0f;

// Канал трассировки
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
TEnumAsByte<ETraceTypeQuery> TraceChannel = ECC_Visibility;

// Текущий целевой объект
AInteractableObject* CurrentTargetObject;

// Линия трассировки
FVector StartTrace;
FVector EndTrace;
};
