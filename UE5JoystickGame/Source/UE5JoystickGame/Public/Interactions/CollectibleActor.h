// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectibleActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UAudioComponent;

/**
 * Тип собираемого предмета
 */
UENUM(BlueprintType)
enum class ECollectibleType : uint8
{
	Coin		UMETA(DisplayName = "Монета"),
	Health		UMETA(DisplayName = "Здоровье"),
	Ammo		UMETA(DisplayName = "Патроны"),
	PowerUp		UMETA(DisplayName = "Усиление"),
	KeyItem		UMETA(DisplayName = "Ключевой предмет")
};

/**
 * Собираемый предмет (монеты, аптечки, бонусы)
 */
UCLASS()
class UE5JOYSTICKGAME_API ACollectibleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACollectibleActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Сфера коллизии */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;

	/** Визуальный меш */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CollectibleMesh;

	/** Эффект сбора (Niagara) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* CollectionEffect;

	/** Звук сбора */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* CollectionSound;

	/** Тип предмета */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	ECollectibleType CollectibleType;

	/** Количество (для монет, патронов) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int32 Amount;

	/** Стоимость в очках */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int32 ScoreValue;

	/** Восстанавливаемое здоровье */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible", meta = (EditCondition = "CollectibleType == ECollectibleType::Health"))
	float HealthRestoreAmount;

	/** Скорость вращения */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	float RotationSpeed;

	/** Амплитуда парения */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	float FloatAmplitude;

	/** Скорость парения */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	float FloatSpeed;

	/** Уже собран? */
	UPROPERTY(BlueprintReadOnly, Category = "Collectible")
	bool bCollected;

	/** Начальная позиция для парения */
	FVector InitialLocation;

	/** Время для анимации парения */
	float FloatTime;

	/** Сбор предмета */
	UFUNCTION(BlueprintCallable, Category = "Collectible")
	void Collect(AActor* Collector);

	/** Воспроизведение эффекта сбора */
	UFUNCTION(BlueprintCallable, Category = "Collectible")
	void PlayCollectionEffect();

	/** Обработка начала перекрытия */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
