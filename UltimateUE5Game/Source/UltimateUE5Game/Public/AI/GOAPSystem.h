// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GOAPSystem.generated.h"

/**
 * GOAP Action - Represents an action the AI can take
 */
UCLASS(BlueprintType)
class ULTIMATEUE5GAME_API UGOAPAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	FString ActionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	float Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	TMap<FString, float> Preconditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	TMap<FString, float> Effects;

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	bool CanExecute(const TMap<FString, float>& WorldState) const;

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void Execute(class APawn* Executor);
};

/**
 * GOAP Goal - Represents a goal the AI wants to achieve
 */
UCLASS(BlueprintType)
class ULTIMATEUE5GAME_API UGOAPGoal : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	FString GoalName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	TMap<FString, float> DesiredState;

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	bool IsSatisfied(const TMap<FString, float>& WorldState) const;
};

/**
 * GOAP System - Plans and executes action sequences
 */
UCLASS()
class ULTIMATEUE5GAME_API UGOAPSystem : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	static TArray<FString> PlanActions(
		const TArray<UGOAPAction*>& AvailableActions,
		const TMap<FString, float>& CurrentState,
		const TMap<FString, float>& GoalState
	);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	static bool IsValidPlan(const TArray<FString>& Plan, const TArray<UGOAPAction*>& Actions);

private:
	static bool FindPath(
		const TArray<UGOAPAction*>& Actions,
		TMap<FString, float> CurrentState,
		const TMap<FString, float>& GoalState,
		TArray<FString>& OutPlan,
		int32 Depth
	);
};
