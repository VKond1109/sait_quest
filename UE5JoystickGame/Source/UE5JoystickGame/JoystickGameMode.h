// Copyright 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JoystickGameMode.generated.h"

UCLASS()
class UE5JOYSTICKGAME_API AJoystickGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AJoystickGameMode();

protected:
	virtual void BeginPlay() override;
};
