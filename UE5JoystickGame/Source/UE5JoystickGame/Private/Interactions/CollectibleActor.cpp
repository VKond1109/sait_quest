// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactions/CollectibleActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "AudioComponents/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACollectibleActor::ACollectibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Сфера коллизии
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectibleActor::OnOverlapBegin);
	RootComponent = SphereComponent;

	// Визуальный меш
	CollectibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleMesh"));
	CollectibleMesh->SetupAttachment(RootComponent);
	CollectibleMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// Эффект сбора (опционально)
	CollectionEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CollectionEffect"));
	CollectionEffect->SetupAttachment(RootComponent);
	CollectionEffect->bAutoActivate = false;

	// Звук сбора
	CollectionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectionSound"));
	CollectionSound->SetupAttachment(RootComponent);
	CollectionSound->bAutoActivate = false;

	// Настройки по умолчанию
	CollectibleType = ECollectibleType::Coin;
	Amount = 1;
	ScoreValue = 10;
	HealthRestoreAmount = 25.0f;
	RotationSpeed = 90.0f;
	FloatAmplitude = 20.0f;
	FloatSpeed = 2.0f;
	bCollected = false;
	FloatTime = 0.0f;

	// Текст взаимодействия
	InteractionText = FText::FromString(TEXT("Собрать"));
}

// Called when the game starts or when spawned
void ACollectibleActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	
	// Установка текста в зависимости от типа
	switch (CollectibleType)
	{
	case ECollectibleType::Coin:
		InteractionText = FText::FromString(TEXT("Монета"));
		break;
	case ECollectibleType::Health:
		InteractionText = FText::FromString(TEXT("Аптечка"));
		break;
	case ECollectibleType::Ammo:
		InteractionText = FText::FromString(TEXT("Патроны"));
		break;
	case ECollectibleType::PowerUp:
		InteractionText = FText::FromString(TEXT("Бонус"));
		break;
	case ECollectibleType::KeyItem:
		InteractionText = FText::FromString(TEXT("Предмет"));
		break;
	}
}

// Called every frame
void ACollectibleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCollected)
		return;

	// Вращение предмета
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += RotationSpeed * DeltaTime;
	SetActorRotation(NewRotation);

	// Парение вверх-вниз
	FloatTime += DeltaTime;
	float FloatOffset = FMath::Sin(FloatTime * FloatSpeed) * FloatAmplitude;
	FVector NewLocation = InitialLocation;
	NewLocation.Z += FloatOffset;
	SetActorLocation(NewLocation);
}

void ACollectibleActor::Collect(AActor* Collector)
{
	if (bCollected || !Collector)
		return;

	bCollected = true;

	UE_LOG(LogTemp, Log, TEXT("Собран предмет: %s, тип: %d, количество: %d"), 
		*GetName(), static_cast<int>(CollectibleType), Amount);

	// Воспроизведение эффектов
	PlayCollectionEffect();

	// Скрываем предмет
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// Уничтожаем через небольшое время (для завершения эффектов)
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		Destroy();
	}, 1.0f, false);
}

void ACollectibleActor::PlayCollectionEffect()
{
	// Воспроизведение звукового эффекта
	if (CollectionSound && CollectionSound->SoundCue)
	{
		CollectionSound->Play();
	}

	// Воспроизведение визуального эффекта
	if (CollectionEffect && CollectionEffect->GetSystemTemplate())
	{
		CollectionEffect->Activate(true);
	}
}

void ACollectibleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || bCollected)
		return;

	// Проверяем, что это игрок
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		Collect(OtherActor);
	}
}
