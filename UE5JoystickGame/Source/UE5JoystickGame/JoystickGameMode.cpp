// Copyright 2024. All rights reserved.

#include "JoystickGameMode.h"
#include "JoystickPlayerCharacter.h"

AJoystickGameMode::AJoystickGameMode()
{
	// Set default pawn class to our custom character
	DefaultPawnClass = AJoystickPlayerCharacter::StaticClass();
}

void AJoystickGameMode::BeginPlay()
{
	Super::BeginPlay();
}
