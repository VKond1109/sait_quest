// Copyright 2024 Ultimate Game Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UJoystickComponent.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ULTIMATEGAME_API UJoystickComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UJoystickComponent();

protected:
	virtual void BeginPlay() override;

public:	
	// Virtual Joystick Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
	float DeadZoneRadius = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
	float MaxOutputValue = 1.0f;

	// Input Actions
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SwitchCameraAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;

	// Output vectors for movement and look
	UPROPERTY(BlueprintReadOnly, Category = "Output")
	FVector2D MovementVector;

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	FVector2D LookVector;

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	bool bIsJumpPressed;

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	bool bIsSprintPressed;

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	bool bIsDashPressed;

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	bool bIsInteractPressed;

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	bool bIsAttackPressed;

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	bool bIsSwitchCameraPressed;

	UPROPERTY(BlueprintReadOnly, Category = "Output")
	bool bIsCrouchPressed;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Joystick")
	void SetMovementVector(FVector2D NewVector);

	UFUNCTION(BlueprintCallable, Category = "Joystick")
	void SetLookVector(FVector2D NewVector);

	UFUNCTION(BlueprintCallable, Category = "Joystick")
	void ResetAllInputs();

private:
	// Touch handling
	int32 ActiveTouchIndex;
	bool bHasActiveTouch;
	FVector2D TouchStartPosition;
	FVector2D TouchCurrentPosition;

	// Keyboard handling
	void HandleKeyboardInput();
};
