// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Интерфейс для всех взаимодействуемых объектов в игре
 */
class UE5JOYSTICKGAME_API IInteractableInterface
{
	GENERATED_BODY()

public:
	/**
	 * Вызывается когда игрок начинает смотреть на объект
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnLookAtStart(AActor* LookingActor);

	/**
	 * Вызывается когда игрок перестает смотреть на объект
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnLookAtEnd(AActor* LookingActor);

	/**
	 * Основное взаимодействие (открыть, подобрать, нажать)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* InteractingActor);

	/**
	 * Возвращает текст подсказки для взаимодействия
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Interaction")
	FText GetInteractionText() const;

	/**
	 * Возвращает иконку для взаимодействия (опционально)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Interaction")
	UTexture2D* GetInteractionIcon() const;
};
