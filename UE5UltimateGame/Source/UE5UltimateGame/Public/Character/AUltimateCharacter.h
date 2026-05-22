// Copyright 2024 Ultimate Game Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AUltimateCharacter.generated.h"

class UJoystickComponent;
class UCameraComponent;
class USpringArmComponent;
class UHealthComponent;
class USurvivalComponent;
class UParkourComponent;
class UStaminaComponent;

UCLASS()
class UE5ULTIMATEGAME_API AUltimateCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUltimateCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UJoystickComponent* JoystickComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* ThirdPersonSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USurvivalComponent* SurvivalComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParkourComponent* ParkourComponent;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bIsFirstPersonView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraSwitchCooldown = 0.5f;

	float LastCameraSwitchTime;

	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 MaxDoubleJumps = 2;

	int32 CurrentDoubleJumps;
	bool bIsDashing;
	float DashStartTime;

	// Combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown = 0.8f;

	float LastAttackTime;

	// Interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange = 300.0f;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SwitchCamera();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void PerformJump();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void PerformSprint(bool bSprinting);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void PerformDash();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void PerformAttack();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void PerformInteract();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void MoveCharacter(FVector2D MovementVector);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void LookCharacter(FVector2D LookVector);

	UFUNCTION(BlueprintCallable, Category = "Character")
	bool CanInteract();

	UFUNCTION(BlueprintCallable, Category = "Character")
	AActor* GetFocusedInteractable();

private:
	void UpdateCamera();
	void UpdateMovement();
	void HandleJump();
	void HandleSprint(bool bSprinting);
	void HandleDash();
	void HandleAttack();
	void HandleInteract();
};
