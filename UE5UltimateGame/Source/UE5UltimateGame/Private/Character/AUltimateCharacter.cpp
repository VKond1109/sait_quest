// Copyright 2024 Ultimate Game Studio. All Rights Reserved.

#include "Character/AUltimateCharacter.h"
#include "Character/UJoystickComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Combat/UHealthComponent.h"
#include "Survival/USurvivalComponent.h"
#include "Parkour/UParkourComponent.h"
#include "Interaction/UInteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AUltimateCharacter::AUltimateCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create joystick component
	JoystickComponent = CreateDefaultSubobject<UJoystickComponent>(TEXT("JoystickComponent"));

	// Setup capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Create camera components
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonSpringArm"));
	ThirdPersonSpringArm->SetupAttachment(GetCapsuleComponent());
	ThirdPersonSpringArm->TargetArmLength = 400.0f;
	ThirdPersonSpringArm->bUsePawnControlRotation = true;
	ThirdPersonSpringArm->bEnableCameraLag = true;
	ThirdPersonSpringArm->CameraLagSpeed = 10.0f;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	// Default to third person
	bIsFirstPersonView = false;
	LastCameraSwitchTime = 0.0f;

	// Movement defaults
	CurrentDoubleJumps = 0;
	bIsDashing = false;
	DashStartTime = 0.0f;

	// Combat defaults
	LastAttackTime = 0.0f;

	// Create additional components (will be implemented)
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	SurvivalComponent = CreateDefaultSubobject<USurvivalComponent>(TEXT("SurvivalComponent"));
	ParkourComponent = CreateDefaultSubobject<UParkourComponent>(TEXT("ParkourComponent"));

	// Set default character movement
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
}

void AUltimateCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize components
	if (HealthComponent)
	{
		HealthComponent->InitializeComponent();
	}

	if (SurvivalComponent)
	{
		SurvivalComponent->InitializeComponent();
	}

	if (ParkourComponent)
	{
		ParkourComponent->InitializeComponent();
	}

	// Set initial camera
	UpdateCamera();
}

void AUltimateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle dash timer
	if (bIsDashing)
	{
		float TimeSinceDash = GetWorld()->GetTimeSeconds() - DashStartTime;
		if (TimeSinceDash >= DashDuration)
		{
			bIsDashing = false;
			GetCharacterMovement()->MaxWalkSpeed = bIsSprintPressed ? SprintSpeed : WalkSpeed;
		}
	}

	// Reset jump counter when grounded
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		CurrentDoubleJumps = MaxDoubleJumps;
	}
}

void AUltimateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input is handled by JoystickComponent
	// This is for legacy input or additional bindings
}

void AUltimateCharacter::SwitchCamera()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastCameraSwitchTime < CameraSwitchCooldown)
	{
		return;
	}

	bIsFirstPersonView = !bIsFirstPersonView;
	LastCameraSwitchTime = CurrentTime;

	UpdateCamera();
}

void AUltimateCharacter::UpdateCamera()
{
	if (bIsFirstPersonView)
	{
		FirstPersonCamera->SetActive(true);
		ThirdPersonCamera->SetActive(false);
		GetMesh()->SetOwnerNoSee(true); // Hide character mesh in first person
	}
	else
	{
		FirstPersonCamera->SetActive(false);
		ThirdPersonCamera->SetActive(true);
		GetMesh()->SetOwnerNoSee(false); // Show character mesh in third person
	}
}

void AUltimateCharacter::PerformJump()
{
	if (bIsDashing)
	{
		return; // Can't jump while dashing
	}

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		// First jump
		LaunchCharacter(FVector(0, 0, GetCharacterMovement()->JumpZVelocity), false, true);
		CurrentDoubleJumps = MaxDoubleJumps - 1;
	}
	else if (CurrentDoubleJumps > 0)
	{
		// Double jump
		LaunchCharacter(FVector(0, 0, GetCharacterMovement()->JumpZVelocity * 0.8f), false, true);
		CurrentDoubleJumps--;
		
		// Add double jump effect here (particles, sound)
	}
}

void AUltimateCharacter::PerformSprint(bool bSprinting)
{
	if (bIsDashing)
	{
		return;
	}

	if (bSprinting && SurvivalComponent && SurvivalComponent->HasEnoughStaminaForSprint())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		SurvivalComponent->ConsumeStamina(10.0f); // Drain stamina while sprinting
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AUltimateCharacter::PerformDash()
{
	if (bIsDashing || !SurvivalComponent || !SurvivalComponent->HasEnoughStaminaForDash())
	{
		return;
	}

	bIsDashing = true;
	DashStartTime = GetWorld()->GetTimeSeconds();
	
	// Consume stamina for dash
	SurvivalComponent->ConsumeStamina(25.0f);

	// Set dash speed
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;

	// Launch forward
	FVector ForwardVector = GetControlRotation().Vector();
	ForwardVector.Z = 0;
	ForwardVector.Normalize();
	
	LaunchCharacter(ForwardVector * DashSpeed * 0.5f, false, true);

	// Add dash effect here (particles, sound)
}

void AUltimateCharacter::PerformAttack()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastAttackTime < AttackCooldown)
	{
		return;
	}

	LastAttackTime = CurrentTime;

	// Perform line trace for attack
	FVector Start = GetActorLocation();
	FVector Forward = GetControlRotation().Vector();
	FVector End = Start + Forward * AttackRange;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
	{
		if (HitResult.GetActor() && HitResult.GetActor() != this)
		{
			// Apply damage to hit actor
			// This would call a damage interface on the target
			UE_LOG(LogTemp, Log, TEXT("Attack hit: %s"), *HitResult.GetActor()->GetName());
			
			// Add hit effect here (particles, sound, camera shake)
		}
	}

	// Add attack animation/montage here
}

void AUltimateCharacter::PerformInteract()
{
	AActor* Interactable = GetFocusedInteractable();
	if (Interactable)
	{
		// Call interact function on the target
		// This would use an interface like IInteractable
		UE_LOG(LogTemp, Log, TEXT("Interacting with: %s"), *Interactable->GetName());
	}
}

bool AUltimateCharacter::CanInteract()
{
	return GetFocusedInteractable() != nullptr;
}

AActor* AUltimateCharacter::GetFocusedInteractable()
{
	FVector Start = GetActorLocation();
	FVector Forward = GetControlRotation().Vector();
	FVector End = Start + Forward * InteractionRange;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
	{
		if (HitResult.GetActor() && HitResult.GetActor() != this)
		{
			// Check if actor implements interactable interface
			// This would check for IInteractable interface
			return HitResult.GetActor();
		}
	}

	return nullptr;
}

void AUltimateCharacter::MoveCharacter(FVector2D MovementVector)
{
	if (MovementVector.IsNearlyZero())
	{
		return;
	}

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	FVector Forward = ControlRot.RotateVector(FVector(MovementVector.Y, 0, 0));
	FVector Right = ControlRot.RotateVector(FVector(0, MovementVector.X, 0));
	
	FVector Movement = (Forward + Right).GetSafeNormal();
	
	if (!bIsDashing)
	{
		AddMovementInput(Movement);
	}
}

void AUltimateCharacter::LookCharacter(FVector2D LookVector)
{
	if (LookVector.IsNearlyZero())
	{
		return;
	}

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void AUltimateCharacter::UpdateMovement()
{
	// Handled by MoveCharacter and LookCharacter
}

void AUltimateCharacter::HandleJump()
{
	PerformJump();
}

void AUltimateCharacter::HandleSprint(bool bSprinting)
{
	PerformSprint(bSprinting);
}

void AUltimateCharacter::HandleDash()
{
	PerformDash();
}

void AUltimateCharacter::HandleAttack()
{
	PerformAttack();
}

void AUltimateCharacter::HandleInteract()
{
	PerformInteract();
}
