// Copyright 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "JoystickPlayerCharacter.generated.h"

class UVirtualJoystickComponent;

UCLASS()
class UE5JOYSTICKGAME_API AJoystickPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AJoystickPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Movement functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	// Jump function
	void PerformJump();
	
	// Sprint function
	void StartSprint();
	void StopSprint();

protected:
	// Virtual joystick component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVirtualJoystickComponent* VirtualJoystick;

	// Camera boom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 1000.0f;

private:
	bool bIsSprinting;
};
