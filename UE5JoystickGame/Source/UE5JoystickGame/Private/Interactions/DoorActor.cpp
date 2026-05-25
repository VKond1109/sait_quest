// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactions/DoorActor.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Создаем скелетный меш для двери
	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
	DoorMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// Триггер для авто-открытия
	AutoOpenTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AutoOpenTrigger"));
	AutoOpenTrigger->SetupAttachment(RootComponent);
	AutoOpenTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	AutoOpenTrigger->SetCollisionProfileName(TEXT("Trigger"));
	AutoOpenTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::OnTriggerBeginOverlap);
	AutoOpenTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoorActor::OnTriggerEndOverlap);

	// Настройки двери
	DoorState = EDoorState::Closed;
	OpenAngle = 90.0f;
	OpenSpeed = 2.0f;
	bAutoOpen = true;
	bAutoClose = true;
	AutoCloseDelay = 3.0f;
	CloseTimer = 0.0f;

	// Текст взаимодействия
	InteractionText = FText::FromString(TEXT("[E] Открыть дверь"));
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorRotation();
	TargetRotation = InitialRotation;
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DoorState == EDoorState::Opening || DoorState == EDoorState::Closing)
	{
		// Интерполяция вращения двери
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, OpenSpeed);
		SetActorRotation(NewRotation);

		// Проверка достижения целевого вращения
		float AngleDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw));
		if (AngleDiff < 1.0f)
		{
			SetActorRotation(TargetRotation);
			DoorState = (DoorState == EDoorState::Opening) ? EDoorState::Open : EDoorState::Closed;
			
			if (DoorState == EDoorState::Closed)
			{
				InteractionText = FText::FromString(TEXT("[E] Открыть дверь"));
			}
			else
			{
				InteractionText = FText::FromString(TEXT("[E] Закрыть дверь"));
			}
		}
	}

	// Таймер авто-закрытия
	if (bAutoClose && DoorState == EDoorState::Open)
	{
		CloseTimer += DeltaTime;
		if (CloseTimer >= AutoCloseDelay)
		{
			CloseDoor();
		}
	}
}

void ADoorActor::Interact(AActor* InteractingActor)
{
	if (!bCanInteract)
		return;

	ToggleDoor();
}

void ADoorActor::OpenDoor()
{
	if (DoorState == EDoorState::Closed || DoorState == EDoorState::Closing)
	{
		DoorState = EDoorState::Opening;
		
		// Вычисляем целевое вращение
		TargetRotation = InitialRotation;
		TargetRotation.Yaw += OpenAngle;
		
		CloseTimer = 0.0f;
		InteractionText = FText::FromString(TEXT("Открытие..."));
	}
}

void ADoorActor::CloseDoor()
{
	if (DoorState == EDoorState::Open || DoorState == EDoorState::Opening)
	{
		DoorState = EDoorState::Closing;
		TargetRotation = InitialRotation;
		InteractionText = FText::FromString(TEXT("Закрытие..."));
	}
}

void ADoorActor::ToggleDoor()
{
	if (DoorState == EDoorState::Closed || DoorState == EDoorState::Closing)
	{
		OpenDoor();
	}
	else
	{
		CloseDoor();
	}
}

void ADoorActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAutoOpen && OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
	{
		OpenDoor();
	}
}

void ADoorActor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
	{
		// Не закрываем сразу, даем время уйти из зоны
	}
}
