// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BaseEnemyCharacter.h"
#include "Combat/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AudioComponents/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Компонент здоровья
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->MaxHealth = 100.0f;
	HealthComponent->SetupAttachment(RootComponent);

	// Меш врага
	EnemyMesh = GetMesh();
	EnemyMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Звуковые компоненты
	AttackSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackSound"));
	AttackSound->bAutoActivate = false;
	AttackSound->SetupAttachment(RootComponent);

	DeathSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathSound"));
	DeathSound->bAutoActivate = false;
	DeathSound->SetupAttachment(RootComponent);

	// Настройки ИИ
	EnemyState = EEnemyState::Idle;
	DetectionRange = 1500.0f;
	AttackRange = 200.0f;
	AttackDamage = 20.0f;
	PatrolSpeed = 100.0f;
	ChaseSpeed = 350.0f;
	AttackCooldown = 1.5f;
	AttackTimer = 0.0f;
	Target = nullptr;

	// Настройки награды
	ScoreReward = 100;
	DropChance = 50.0f;

	// Авто-создание AIController
	AIControllerClass = AAIController::StaticClass();
}

// Called when the game starts or when spawned
void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Привязка событий здоровья
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ABaseEnemyCharacter::OnTakeDamage);
		HealthComponent->OnDeath.AddDynamic(this, &ABaseEnemyCharacter::OnDeath);
	}

	// Запуск Behavior Tree
	if (BehaviorTree && HasAuthority())
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->RunBehaviorTree(BehaviorTree);
			
			if (BlackboardData)
			{
				AIController->GetBlackboardComponent()->InitializeBlackboard(*BlackboardData);
			}
		}
	}

	// Установка тега для взаимодействия
	Tags.Add(FName("Enemy"));
}

// Called every frame
void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Таймер атаки
	if (AttackTimer > 0.0f)
	{
		AttackTimer -= DeltaTime;
	}

	// Обновление состояния в Blackboard
	if (HasAuthority())
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController && AIController->GetBlackboardComponent())
		{
			AIController->GetBlackboardComponent()->SetValueAsEnum(FName("EnemyState"), static_cast<uint8>(EnemyState));
		}
	}
}

void ABaseEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemyCharacter::PerformAttack()
{
	if (AttackTimer > 0.0f || !Target)
		return;

	EnemyState = EEnemyState::Attack;
	AttackTimer = AttackCooldown;

	// Воспроизведение звука
	if (AttackSound && AttackSound->SoundCue)
	{
		AttackSound->Play();
	}

	// Воспроизведение анимации
	if (AttackMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	}

	// Нанесение урона через небольшую задержку (для синхронизации с анимацией)
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		DealDamage();
	}, 0.3f, false);
}

void ABaseEnemyCharacter::DealDamage()
{
	if (!Target)
		return;

	// Проверка расстояния
	float Distance = GetDistanceToTarget();
	if (Distance > AttackRange * 1.5f)
		return;

	// Получаем компонент здоровья цели
	UHealthComponent* TargetHealth = Target->FindComponentByClass<UHealthComponent>();
	if (TargetHealth)
	{
		TargetHealth->TakeDamage(AttackDamage, GetController(), this);
		UE_LOG(LogTemp, Log, TEXT("Враг нанес %f урона цели"), AttackDamage);
	}
}

void ABaseEnemyCharacter::OnTakeDamage(float NewHealth, float MaxHealth, float Delta, AActor* DamageCauser)
{
	if (Delta < 0.0f && EnemyState != EEnemyState::Dead)
	{
		// Переключаемся на преследование при получении урона
		if (DamageCauser)
		{
			Target = DamageCauser;
			EnemyState = EEnemyState::Chase;
		}
	}
}

void ABaseEnemyCharacter::OnDeath(AActor* Victim, AController* InstigatorController)
{
	EnemyState = EEnemyState::Dead;

	// Воспроизведение анимации смерти
	if (DeathMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
	}

	// Воспроизведение звука смерти
	if (DeathSound && DeathSound->SoundCue)
	{
		DeathSound->Play();
	}

	// Отключение коллизии и физики
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	// Выпадение предмета
	if (DropItemClass && FMath::FRandRange(0.0f, 100.0f) <= DropChance)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += 50.0f;
		
		GetWorld()->SpawnActor<AActor>(DropItemClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	}

	// Уничтожение через время
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		Destroy();
	}, 5.0f, false);
}

float ABaseEnemyCharacter::GetCurrentSpeed() const
{
	switch (EnemyState)
	{
	case EEnemyState::Patrol:
		return PatrolSpeed;
	case EEnemyState::Chase:
		return ChaseSpeed;
	default:
		return 0.0f;
	}
}

bool ABaseEnemyCharacter::CanSeeTarget() const
{
	if (!Target)
		return false;

	FHitResult HitResult;
	FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
	FVector End = Target->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bVisible = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	return bVisible && HitResult.GetActor() == Target;
}

float ABaseEnemyCharacter::GetDistanceToTarget() const
{
	if (!Target)
		return FLT_MAX;

	return FVector::Dist(GetActorLocation(), Target->GetActorLocation());
}
