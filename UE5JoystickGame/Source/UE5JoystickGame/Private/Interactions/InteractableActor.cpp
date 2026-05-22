// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactions/InteractableActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Создаем компонент коллизии
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	InteractionBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = InteractionBox;

	// Создаем статический меш
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// Создаем текст подсказки
	HintText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HintText"));
	HintText->SetupAttachment(RootComponent);
	HintText->SetTextRenderColor(FColor::Yellow);
	HintText->SetText(FText::FromString(TEXT("[E]")));
	HintText->SetVisibility(false);

	// Значения по умолчанию
	InteractionText = FText::FromString(TEXT("Взаимодействовать"));
	InteractionIcon = nullptr;
	bCanInteract = true;
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
	HintText->SetVisibility(false);
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActor::Interact(AActor* InteractingActor)
{
	if (!bCanInteract || !InteractingActor)
		return;

	// Базовая реализация - переопределяется в дочерних классах
	UE_LOG(LogTemp, Log, TEXT("%s взаимодействует с %s"), 
		*InteractingActor->GetName(), *GetName());
}

void AInteractableActor::OnLookStart(AActor* LookingActor)
{
	if (HintText)
	{
		HintText->SetVisibility(true);
		HintText->SetText(InteractionText);
	}
}

void AInteractableActor::OnLookEnd(AActor* LookingActor)
{
	if (HintText)
	{
		HintText->SetVisibility(false);
	}
}

bool AInteractableActor::IsVisibleBy(AActor* Viewer) const
{
	if (!Viewer || !StaticMesh)
		return false;

	FHitResult HitResult;
	FVector Start = Viewer->GetActorLocation();
	FVector End = GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Viewer);
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	return bHit && HitResult.GetActor() == this;
}
