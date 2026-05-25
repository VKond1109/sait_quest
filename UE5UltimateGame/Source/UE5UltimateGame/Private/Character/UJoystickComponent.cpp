// Copyright 2024 Ultimate Game Studio. All Rights Reserved.

#include "Character/UJoystickComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

UJoystickComponent::UJoystickComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	DeadZoneRadius = 0.1f;
	MaxOutputValue = 1.0f;
	
	MovementVector = FVector2D::ZeroVector;
	LookVector = FVector2D::ZeroVector;
	
	bIsJumpPressed = false;
	bIsSprintPressed = false;
	bIsDashPressed = false;
	bIsInteractPressed = false;
	bIsAttackPressed = false;
	bIsSwitchCameraPressed = false;
	bIsCrouchPressed = false;
	
	ActiveTouchIndex = -1;
	bHasActiveTouch = false;
}

void UJoystickComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup input for the player controller
	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				FModifyContextOptions Options;
				Options.bIgnoreAllPhasedOutInputs = true;
				Subsystem->AddMappingContext(DefaultMappingContext, Options);
			}
		}
		
		// Bind input actions
		if (UEnhancedInputComponent* InputComponent = PlayerController->FindComponentByClass<UEnhancedInputComponent>())
		{
			if (MoveAction)
			{
				InputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UJoystickComponent::SetMovementVector);
				InputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, [this]() { SetMovementVector(FVector2D::ZeroVector); });
			}
			
			if (LookAction)
			{
				InputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UJoystickComponent::SetLookVector);
				InputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, [this]() { SetLookVector(FVector2D::ZeroVector); });
			}
			
			if (JumpAction)
			{
				InputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, [this]() { bIsJumpPressed = true; });
				InputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, [this]() { bIsJumpPressed = false; });
			}
			
			if (SprintAction)
			{
				InputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, [this]() { bIsSprintPressed = true; });
				InputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, [this]() { bIsSprintPressed = false; });
			}
			
			if (DashAction)
			{
				InputComponent->BindAction(DashAction, ETriggerEvent::Started, this, [this]() { bIsDashPressed = true; });
				InputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, [this]() { bIsDashPressed = false; });
			}
			
			if (InteractAction)
			{
				InputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, [this]() { bIsInteractPressed = true; });
				InputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, [this]() { bIsInteractPressed = false; });
			}
			
			if (AttackAction)
			{
				InputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, [this]() { bIsAttackPressed = true; });
				InputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, [this]() { bIsAttackPressed = false; });
			}
			
			if (SwitchCameraAction)
			{
				InputComponent->BindAction(SwitchCameraAction, ETriggerEvent::Started, this, [this]() { bIsSwitchCameraPressed = true; });
				InputComponent->BindAction(SwitchCameraAction, ETriggerEvent::Completed, this, [this]() { bIsSwitchCameraPressed = false; });
			}
			
			if (CrouchAction)
			{
				InputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, [this]() { bIsCrouchPressed = true; });
				InputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, [this]() { bIsCrouchPressed = false; });
			}
		}
	}
}

void UJoystickComponent::SetMovementVector(FVector2D NewVector)
{
	if (NewVector.Size() < DeadZoneRadius)
	{
		MovementVector = FVector2D::ZeroVector;
	}
	else
	{
		MovementVector = (NewVector - DeadZoneRadius) / (1.0f - DeadZoneRadius);
		MovementVector = FMath::ClampVector(MovementVector, FVector2D::ZeroVector, FVector2D(MaxOutputValue, MaxOutputValue));
	}
}

void UJoystickComponent::SetLookVector(FVector2D NewVector)
{
	if (NewVector.Size() < DeadZoneRadius)
	{
		LookVector = FVector2D::ZeroVector;
	}
	else
	{
		LookVector = (NewVector - DeadZoneRadius) / (1.0f - DeadZoneRadius);
		LookVector = FMath::ClampVector(LookVector, FVector2D::ZeroVector, FVector2D(MaxOutputValue, MaxOutputValue));
	}
}

void UJoystickComponent::ResetAllInputs()
{
	MovementVector = FVector2D::ZeroVector;
	LookVector = FVector2D::ZeroVector;
	
	bIsJumpPressed = false;
	bIsSprintPressed = false;
	bIsDashPressed = false;
	bIsInteractPressed = false;
	bIsAttackPressed = false;
	bIsSwitchCameraPressed = false;
	bIsCrouchPressed = false;
}

void UJoystickComponent::HandleKeyboardInput()
{
	// This would handle keyboard input fallback if needed
	// Enhanced Input system handles most of this automatically
}
