// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/InteractableActor.h"
#include "DoorActor.generated.h"

class UBoxComponent;
class USkeletalMeshComponent;

/**
 * Состояние двери
 */
UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed		UMETA(DisplayName = "Закрыта"),
	Opening		UMETA(DisplayName = "Открывается"),
	Open		UMETA(DisplayName = "Открыта"),
	Closing		UMETA(DisplayName = "Закрывается")
};

/**
 * Актер двери с анимацией открытия/закрытия
 */
UCLASS()
class UE5JOYSTICKGAME_API ADoorActor : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	ADoorActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Дверь (скелетный меш для анимации) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* DoorMesh;

	/** Компонент триггера для авто-открытия */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* AutoOpenTrigger;

	/** Текущее состояние двери */
	UPROPERTY(BlueprintReadOnly, Category = "Door")
	EDoorState DoorState;

	/** Угол открытия двери (в градусах) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenAngle;

	/** Скорость открытия/закрытия */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenSpeed;

	/** Автоматически открываться при приближении игрока */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bAutoOpen;

	/** Закрывать дверь автоматически после ухода игрока */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bAutoClose;

	/** Задержка перед автоматическим закрытием */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (EditCondition = "bAutoClose"))
	float AutoCloseDelay;

	/** Таймер для авто-закрытия */
	float CloseTimer;

	/** Начальная ротация двери */
	FRotator InitialRotation;

	/** Целевая ротация */
	FRotator TargetRotation;

	/** Открыть дверь */
	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor();

	/** Закрыть дверь */
	UFUNCTION(BlueprintCallable, Category = "Door")
	void CloseDoor();

	/** Переключить состояние двери */
	UFUNCTION(BlueprintCallable, Category = "Door")
	void ToggleDoor();

	/** Обработка попадания в триггер авто-открытия */
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Обработка выхода из триггера авто-открытия */
	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
