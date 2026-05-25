// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

/**
 * Базовый класс для всех взаимодействуемых объектов
 * Примеры: двери, сундуки, кнопки, подбираемые предметы
 */
UCLASS()
class UE5JOYSTICKGAME_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Компонент коллизии для обнаружения взгляда игрока */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* InteractionBox;

	/** Статический меш (визуальное представление) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	/** Текст подсказки (для отладки) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTextRenderComponent* HintText;

	/** Текст взаимодействия */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionText;

	/** Иконка взаимодействия */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UTexture2D* InteractionIcon;

	/** Можно ли взаимодействовать прямо сейчас */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	bool bCanInteract;

	/** Тип взаимодействия */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact(AActor* InteractingActor);

	/** Начало взгляда на объект */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnLookStart(AActor* LookingActor);

	/** Конец взгляда на объект */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnLookEnd(AActor* LookingActor);

	/** Проверка видимости игроком */
	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool IsVisibleBy(AActor* Viewer) const;
};
