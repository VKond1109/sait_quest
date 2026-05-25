#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// Интерфейс для всех взаимодействуемых объектов
UINTERFACE(MinimalAPI, BlueprintType)
class UInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

class UE5ULTIMATEGAME_API IInteractableInterface
{
    GENERATED_BODY()

public:
    // Основной метод взаимодействия
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void Interact();

    // Возвращает текст подсказки для взаимодействия
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    FString GetInteractionText() const;

    // Можно ли взаимодействовать с этим объектом
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    bool CanInteract(AActor* Interactor) const;

    // Расстояние взаимодействия
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    float GetInteractionDistance() const;
};
