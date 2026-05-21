// Copyright 2024. All rights reserved.

#include "VirtualJoystickComponent.h"

UVirtualJoystickComponent::UVirtualJoystickComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	DeadZone = 0.1f;
	MaxRadius = 100.0f;
	bIsMobile = false;
	AxisValue = FVector2D::ZeroVector;
	bIsPressed = false;
	TouchStartPosition = FVector2D::ZeroVector;
	TouchCurrentPosition = FVector2D::ZeroVector;
	ActiveTouchIndex = -1;
	bHasActiveTouch = false;
}

void UVirtualJoystickComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Detect if we're on a mobile platform
#if PLATFORM_IOS || PLATFORM_ANDROID
	bIsMobile = true;
#endif
}

void UVirtualJoystickComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bHasActiveTouch && bIsMobile)
	{
		// Calculate the delta from the start position
		FVector2D Delta = TouchCurrentPosition - TouchStartPosition;
		
		// Clamp to max radius
		float Distance = Delta.Size();
		if (Distance > MaxRadius)
		{
			Delta = Delta.GetSafeNormal() * MaxRadius;
		}

		// Normalize to -1 to 1 range
		AxisValue = Delta / MaxRadius;

		// Apply deadzone
		if (AxisValue.SizeSquared() < DeadZone * DeadZone)
		{
			AxisValue = FVector2D::ZeroVector;
			bIsPressed = false;
		}
		else
		{
			bIsPressed = true;
		}
	}
	else if (!bIsMobile)
	{
		// For desktop, axis values are set via OnAnalogInput from keyboard/gamepad
		if (AxisValue.SizeSquared() < DeadZone * DeadZone)
		{
			AxisValue = FVector2D::ZeroVector;
			bIsPressed = false;
		}
		else
		{
			bIsPressed = true;
		}
	}
}

void UVirtualJoystickComponent::OnTouchStarted(FVector2D TouchPosition)
{
	if (!bHasActiveTouch)
	{
		TouchStartPosition = TouchPosition;
		TouchCurrentPosition = TouchPosition;
		bHasActiveTouch = true;
		bIsPressed = true;
	}
}

void UVirtualJoystickComponent::OnTouchMoved(FVector2D TouchPosition)
{
	if (bHasActiveTouch)
	{
		TouchCurrentPosition = TouchPosition;
	}
}

void UVirtualJoystickComponent::OnTouchEnded()
{
	bHasActiveTouch = false;
	bIsPressed = false;
	AxisValue = FVector2D::ZeroVector;
	ActiveTouchIndex = -1;
}

void UVirtualJoystickComponent::OnAnalogInput(float AxisX, float AxisY)
{
	AxisValue = FVector2D(AxisX, AxisY);
	
	// Clamp to -1 to 1
	AxisValue.X = FMath::Clamp(AxisValue.X, -1.0f, 1.0f);
	AxisValue.Y = FMath::Clamp(AxisValue.Y, -1.0f, 1.0f);
}
