// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UltimateAIController.generated.h"

/**
 * Advanced AI Controller with GOAP and Utility AI support
 */
UCLASS()
class ULTIMATEUE5GAME_API AUltimateAIController : public AAIController
{
	GENERATED_BODY()

public:
	AUltimateAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// GOAP (Goal Oriented Action Planning)
	UFUNCTION(BlueprintCallable, Category = "AI|GOAP")
	void SetGOAPGoal(const FString& GoalName);

	UFUNCTION(BlueprintCallable, Category = "AI|GOAP")
	void AddWorldStateProperty(const FString& Key, float Value);

	// Utility AI
	UFUNCTION(BlueprintCallable, Category = "AI|Utility")
	void EvaluateActions();

	UFUNCTION(BlueprintCallable, Category = "AI|Utility")
	float GetActionScore(const FString& ActionName) const;

	// Behavior Management
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SwitchBehaviorTree(UBehaviorTree* NewBehaviorTree);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetBlackboardValue(const FString& KeyName, const FVariant& Value);

	// Perception & Awareness
	UFUNCTION(BlueprintCallable, Category = "AI|Perception")
	void SetAwarenessLevel(float Level); // 0.0 to 1.0

	UFUNCTION(BlueprintCallable, Category = "AI|Perception")
	float GetAwarenessLevel() const { return AwarenessLevel; }

	// Stealth Detection
	UFUNCTION(BlueprintCallable, Category = "AI|Stealth")
	bool DetectPlayer(APawn* PlayerPawn) const;

	UFUNCTION(BlueprintCallable, Category = "AI|Stealth")
	void SetLastKnownPlayerLocation(const FVector& Location);

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI|GOAP")
	TMap<FString, TObjectPtr<class UGOAPAction>> AvailableActions;

	UPROPERTY(EditAnywhere, Category = "AI|GOAP")
	TMap<FString, TObjectPtr<class UGOAPGoal>> AvailableGoals;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardComponent> BlackboardComp;

	UPROPERTY()
	TObjectPtr<class UAIPerceptionComponent> PerceptionComp;

	float AwarenessLevel;
	FVector LastKnownPlayerLocation;
	bool bHasKnownPlayerLocation;

	TArray<FString> CurrentGOAPPlan;
	
	void ExecuteGOAPPlan();
	void UpdateUtilityScores();
};
