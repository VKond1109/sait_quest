// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/EnhancedJoystickPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpringArmComponent.h"
#include "Combat/HealthComponent.h"
#include "Interactions/InteractableActor.h"
#include "Interactions/InteractableInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

// Sets default values
AEnhancedJoystickPlayerCharacter::AEnhancedJoystickPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Настройка капсулы
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Камера от 3-го лица
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.0f;

	// Компонент камеры
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Компонент здоровья
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->MaxHealth = 100.0f;
	HealthComponent->HealthRegenRate = 5.0f;
	HealthComponent->SetupAttachment(RootComponent);

	// Настройки движения
	WalkSpeed = 300.0f;
	SprintSpeed = 600.0f;
	MaxDoubleJumps = 1;
	CurrentDoubleJumps = MaxDoubleJumps;
	DashDuration = 0.3f;
	DashForce = 2000.0f;
	DashCooldown = 1.0f;
	DashTimer = 0.0f;
	DashCooldownTimer = 0.0f;
	bIsDashing = false;
	bIsFirstPerson = false;

	// Настройки взаимодействия
	InteractionRange = 300.0f;
	CurrentInteractable = nullptr;

	// Настройки боя
	AttackDamage = 25.0f;
	AttackCooldown = 0.5f;
	AttackTimer = 0.0f;
	bCanAttack = true;

	// Очки
	Score = 0;

	// Тег игрока
	Tags.Add(FName("Player"));
}

// Called when the game starts or when spawned
void AEnhancedJoystickPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Добавляем контекст ввода
	if (DefaultMappingContext)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (Subsystem)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	// Привязка событий здоровья
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AEnhancedJoystickPlayerCharacter::OnDeath);
	}

	// Установка начальной скорости
	UpdateMovementSpeed();
}

// Called every frame
void AEnhancedJoystickPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Обновление взаимодействуемого объекта
	UpdateInteractable();

	// Таймер атаки
	if (AttackTimer > 0.0f)
	{
		AttackTimer -= DeltaTime;
		if (AttackTimer <= 0.0f)
		{
			bCanAttack = true;
		}
	}

	// Таймер рывка
	if (bIsDashing)
	{
		DashTimer -= DeltaTime;
		if (DashTimer <= 0.0f)
		{
			bIsDashing = false;
			GetCharacterMovement()->GravityScale = 1.0f;
		}
	}

	// Кулдаун рывка
	if (DashCooldownTimer > 0.0f)
	{
		DashCooldownTimer -= DeltaTime;
	}

	// Восстановление двойных прыжков на земле
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		CurrentDoubleJumps = MaxDoubleJumps;
	}
}

// Called to bind functionality to input
void AEnhancedJoystickPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Движение
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEnhancedJoystickPlayerCharacter::Move);
		}

		// Вращение камеры
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEnhancedJoystickPlayerCharacter::Look);
		}

		// Прыжок
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AEnhancedJoystickPlayerCharacter::PerformJump);
		}

		// Бег
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AEnhancedJoystickPlayerCharacter::StartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AEnhancedJoystickPlayerCharacter::StopSprint);
		}

		// Взаимодействие
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AEnhancedJoystickPlayerCharacter::PerformInteract);
		}

		// Атака
		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AEnhancedJoystickPlayerCharacter::PerformAttack);
		}

		// Способность (рывок)
		if (AbilityAction)
		{
			EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &AEnhancedJoystickPlayerCharacter::PerformAbility);
		}

		// Смена камеры
		if (ToggleCameraAction)
		{
			EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Started, this, &AEnhancedJoystickPlayerCharacter::ToggleCameraMode);
		}
	}
}

void AEnhancedJoystickPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (bIsDashing || !HealthComponent->IsAlive())
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	FRotator ControlRot = GetControlRotation();
	FRotator YawRotator(0.0f, ControlRot.Yaw, 0.0f);

	FVector ForwardDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AEnhancedJoystickPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (bIsDashing || !HealthComponent->IsAlive())
		return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	// Ограничение вращения камеры по вертикали
	float Pitch = GetControlRotation().Pitch;
	if (Pitch > 89.0f)
	{
		ControlRotation.Pitch = 89.0f;
	}
	else if (Pitch < -89.0f)
	{
		ControlRotation.Pitch = -89.0f;
	}
}

void AEnhancedJoystickPlayerCharacter::PerformJump()
{
	if (bIsDashing || !HealthComponent->IsAlive())
		return;

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		CurrentDoubleJumps = MaxDoubleJumps;
		Jump();
	}
	else if (CurrentDoubleJumps > 0)
	{
		Jump();
		CurrentDoubleJumps--;
	}
}

void AEnhancedJoystickPlayerCharacter::StartSprint()
{
	if (bIsDashing || !HealthComponent->IsAlive())
		return;

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AEnhancedJoystickPlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AEnhancedJoystickPlayerCharacter::PerformInteract()
{
	if (!CurrentInteractable || !HealthComponent->IsAlive())
		return;

	// Проверка расстояния
	float Distance = FVector::Dist(GetActorLocation(), CurrentInteractable->GetActorLocation());
	if (Distance > InteractionRange * 1.2f)
		return;

	// Проверка видимости
	FHitResult HitResult;
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = CurrentInteractable->GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bVisible = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	if (bVisible && HitResult.GetActor() == CurrentInteractable)
	{
		// Вызов интерфейса взаимодействия
		IInteractableInterface* Interactable = Cast<IInteractableInterface>(CurrentInteractable);
		if (Interactable)
		{
			Interactable->Interact(this);
		}
		else
		{
			// Для базовых классов InteractableActor
			AInteractableActor* InteractableActor = Cast<AInteractableActor>(CurrentInteractable);
			if (InteractableActor)
			{
				InteractableActor->Interact(this);
			}
		}
	}
}

void AEnhancedJoystickPlayerCharacter::UpdateInteractable()
{
	FHitResult HitResult;
	FVector Start = FollowCamera->GetComponentLocation();
	FVector Forward = FollowCamera->GetForwardVector();
	FVector End = Start + (Forward * InteractionRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();
		
		// Проверяем, является ли объект взаимодействуемым
		if (HitActor->Implements<UInteractableInterface>() || 
			Cast<AInteractableActor>(HitActor))
		{
			if (CurrentInteractable != HitActor)
			{
				// Предыдущий объект
				if (CurrentInteractable)
				{
					IInteractableInterface* OldInteractable = Cast<IInteractableInterface>(CurrentInteractable);
					if (OldInteractable)
					{
						OldInteractable->OnLookAtEnd(this);
					}
					else
					{
						AInteractableActor* OldActor = Cast<AInteractableActor>(CurrentInteractable);
						if (OldActor)
						{
							OldActor->OnLookEnd(this);
						}
					}
				}

				// Новый объект
				CurrentInteractable = HitActor;
				
				IInteractableInterface* NewInteractable = Cast<IInteractableInterface>(CurrentInteractable);
				if (NewInteractable)
				{
					NewInteractable->OnLookAtStart(this);
				}
				else
				{
					AInteractableActor* NewActor = Cast<AInteractableActor>(CurrentInteractable);
					if (NewActor)
					{
						NewActor->OnLookStart(this);
					}
				}
			}
			return;
		}
	}

	// Сброс если ничего не найдено
	if (CurrentInteractable)
	{
		IInteractableInterface* OldInteractable = Cast<IInteractableInterface>(CurrentInteractable);
		if (OldInteractable)
		{
			OldInteractable->OnLookAtEnd(this);
		}
		else
		{
			AInteractableActor* OldActor = Cast<AInteractableActor>(CurrentInteractable);
			if (OldActor)
			{
				OldActor->OnLookEnd(this);
			}
		}
		CurrentInteractable = nullptr;
	}
}

void AEnhancedJoystickPlayerCharacter::PerformAttack()
{
	if (!bCanAttack || bIsDashing || !HealthComponent->IsAlive())
		return;

	bCanAttack = false;
	AttackTimer = AttackCooldown;

	// Воспроизведение анимации
	if (AttackMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	}

	// Нанесение урона через небольшую задержку
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		// Поиск врагов в радиусе атаки
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors, ABaseEnemyCharacter::StaticClass());

		for (AActor* Actor : OverlappingActors)
		{
			ABaseEnemyCharacter* Enemy = Cast<ABaseEnemyCharacter>(Actor);
			if (Enemy && Enemy->GetHealthComponent())
			{
				Enemy->GetHealthComponent()->TakeDamage(AttackDamage, GetController(), this);
			}
		}
	}, 0.2f, false);
}

void AEnhancedJoystickPlayerCharacter::PerformAbility()
{
	if (DashCooldownTimer > 0.0f || bIsDashing || !HealthComponent->IsAlive())
		return;

	ExecuteDash();
}

void AEnhancedJoystickPlayerCharacter::ExecuteDash()
{
	bIsDashing = true;
	DashTimer = DashDuration;
	DashCooldownTimer = DashCooldown;

	// Получаем направление взгляда
	FRotator ControlRot = GetControlRotation();
	FVector DashDirection = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
	DashDirection.Z = 0.0f;
	DashDirection.Normalize();

	// Применяем импульс
	FVector Impulse = DashDirection * DashForce;
	LaunchCharacter(Impulse, false, true);

	// Отключаем гравитацию на время рывка
	GetCharacterMovement()->GravityScale = 0.0f;

	// Воспроизведение анимации
	if (AbilityMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
	}
}

void AEnhancedJoystickPlayerCharacter::ToggleCameraMode()
{
	bIsFirstPerson = !bIsFirstPerson;

	if (bIsFirstPerson)
	{
		CameraBoom->TargetArmLength = 0.0f;
		CameraBoom->bEnableCameraLag = false;
		FollowCamera->SetRelativeLocation(FVector(10.0f, 0.0f, 64.0f));
	}
	else
	{
		CameraBoom->TargetArmLength = 400.0f;
		CameraBoom->bEnableCameraLag = true;
		FollowCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

void AEnhancedJoystickPlayerCharacter::UpdateMovementSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

int32 AEnhancedJoystickPlayerCharacter::GetScore() const
{
	return Score;
}

void AEnhancedJoystickPlayerCharacter::AddScore(int32 Amount)
{
	Score += Amount;
	OnScoreChanged.Broadcast(Score);
	UE_LOG(LogTemp, Log, TEXT("Очки: %d (+%d)"), Score, Amount);
}

UHealthComponent* AEnhancedJoystickPlayerCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

bool AEnhancedJoystickPlayerCharacter::IsAlive() const
{
	return HealthComponent && HealthComponent->IsAlive();
}

void AEnhancedJoystickPlayerCharacter::OnDeath_Implementation(AActor* Victim, AController* InstigatorController)
{
	// Отключение ввода при смерти
	DisableInput(nullptr);
	
	// Воспроизведение анимации смерти
	// GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
}
