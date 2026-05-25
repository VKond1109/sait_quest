// Copyright 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Components/ActorComponent.h"
#include "VirtualJoystickComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5JOYSTICKGAME_API UVirtualJoystickComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVirtualJoystickComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Joystick properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
	float DeadZone = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
	float MaxRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
	bool bIsMobile = false;

	// Output values
	UPROPERTY(BlueprintReadOnly, Category = "Joystick")
	FVector2D AxisValue = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Joystick")
	bool bIsPressed = false;

	// Touch events
	UFUNCTION(BlueprintCallable, Category = "Joystick")
	void OnTouchStarted(FVector2D TouchPosition);

	UFUNCTION(BlueprintCallable, Category = "Joystick")
	void OnTouchMoved(FVector2D TouchPosition);

	UFUNCTION(BlueprintCallable, Category = "Joystick")
	void OnTouchEnded();

	// Keyboard/Gamepad fallback
	void OnAnalogInput(float AxisX, float AxisY);

	// Get current axis value
	UFUNCTION(BlueprintPure, Category = "Joystick")
	FVector2D GetAxisValue() const { return AxisValue; }

	UFUNCTION(BlueprintPure, Category = "Joystick")
	bool GetIsPressed() const { return bIsPressed; }

private:
	FVector2D TouchStartPosition;
	FVector2D TouchCurrentPosition;
	int32 ActiveTouchIndex;
	bool bHasActiveTouch;
};
