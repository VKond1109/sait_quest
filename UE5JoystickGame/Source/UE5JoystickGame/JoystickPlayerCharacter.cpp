// Copyright 2024. All rights reserved.

#include "JoystickPlayerCharacter.h"
#include "VirtualJoystickComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AJoystickPlayerCharacter::AJoystickPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create virtual joystick component
	VirtualJoystick = CreateDefaultSubobject<UVirtualJoystickComponent>(TEXT("VirtualJoystick"));
	VirtualJoystick->SetupAttachment(RootComponent);

	// Create camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Default movement settings
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bIsSprinting = false;
}

void AJoystickPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Input mapping context would be added here in a full implementation
		}
	}
}

void AJoystickPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update movement based on virtual joystick
	if (VirtualJoystick && VirtualJoystick->GetIsPressed())
	{
		FVector2D AxisValue = VirtualJoystick->GetAxisValue();
		
		// Apply movement input
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue.Y);
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisValue.X);
	}
}

void AJoystickPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up enhanced input
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement binding would be set up here with Enhanced Input System
		// For now, we rely on the virtual joystick component
		
		// Jump
		EnhancedInputComponent->BindAction(/*JumpAction*/, ETriggerEvent::Started, this, &AJoystickPlayerCharacter::PerformJump);
		
		// Sprint
		EnhancedInputComponent->BindAction(/*SprintAction*/, ETriggerEvent::Started, this, &AJoystickPlayerCharacter::StartSprint);
		EnhancedInputComponent->BindAction(/*SprintAction*/, ETriggerEvent::Completed, this, &AJoystickPlayerCharacter::StopSprint);
	}
}

void AJoystickPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AJoystickPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AJoystickPlayerCharacter::PerformJump()
{
	if (bIsSprinting)
	{
		StopSprint();
	}
	Jump();
}

void AJoystickPlayerCharacter::StartSprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AJoystickPlayerCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
