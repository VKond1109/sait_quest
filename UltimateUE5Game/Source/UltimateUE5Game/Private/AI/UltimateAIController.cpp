// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#include "AI/UltimateAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "AI/GOAPSystem.h"

AUltimateAIController::AUltimateAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Setup Blackboard
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	
	// Setup Perception
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	
	auto SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1600.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	
	auto HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->HearingThreshold = 300.0f;
	HearingConfig->LOSHeatingThreshold = 600.0f;
	
	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->ConfigureSense(*HearingConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	
	SetGenericTeamId(FGenericTeamId(0));
	
	AwarenessLevel = 0.5f;
	bHasKnownPlayerLocation = false;
}

void AUltimateAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (DefaultBehaviorTree && BlackboardComp)
	{
		UseBlackboard(DefaultBehaviorTree->BlackboardAsset, BlackboardComp);
		RunBehaviorTree(DefaultBehaviorTree);
	}
	
	UE_LOG(LogTemp, Log, TEXT("AI Controller possessed pawn: %s"), *InPawn->GetName());
}

void AUltimateAIController::OnUnPossess()
{
	Super::OnUnPossess();
	UE_LOG(LogTemp, Log, TEXT("AI Controller unpossessed"));
}

void AUltimateAIController::SetGOAPGoal(const FString& GoalName)
{
	UE_LOG(LogTemp, Log, TEXT("Setting GOAP Goal: %s"), *GoalName);
	
	// Find and activate the goal
	if (AvailableGoals.Contains(GoalName))
	{
		// Generate plan to achieve this goal
		ExecuteGOAPPlan();
	}
}

void AUltimateAIController::AddWorldStateProperty(const FString& Key, float Value)
{
	// Update world state for GOAP planning
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsFloat(Key, Value);
	}
}

void AUltimateAIController::EvaluateActions()
{
	UpdateUtilityScores();
	
	// Select best action based on utility scores
	float BestScore = -FLT_MAX;
	FString BestAction;
	
	for (auto& ActionPair : AvailableActions)
	{
		float Score = GetActionScore(ActionPair.Key);
		if (Score > BestScore)
		{
			BestScore = Score;
			BestAction = ActionPair.Key;
		}
	}
	
	if (!BestAction.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Best Utility Action: %s with score %f"), *BestAction, BestScore);
	}
}

float AUltimateAIController::GetActionScore(const FString& ActionName) const
{
	// Calculate utility score based on current world state
	// Implementation depends on specific action requirements
	return FMath::FRandRange(0.0f, 1.0f);
}

void AUltimateAIController::SwitchBehaviorTree(UBehaviorTree* NewBehaviorTree)
{
	if (NewBehaviorTree)
	{
		StopBehaviorTree();
		RunBehaviorTree(NewBehaviorTree);
		UE_LOG(LogTemp, Log, TEXT("Switched to new Behavior Tree"));
	}
}

void AUltimateAIController::SetBlackboardValue(const FString& KeyName, const FVariant& Value)
{
	if (BlackboardComp)
	{
		// Set blackboard value by key name
		BlackboardComp->SetValueAsVector(KeyName, Value.Get<FVector>());
	}
}

void AUltimateAIController::SetAwarenessLevel(float Level)
{
	AwarenessLevel = FMath::Clamp(Level, 0.0f, 1.0f);
	UE_LOG(LogTemp, Log, TEXT("AI Awareness Level set to: %f"), AwarenessLevel);
}

bool AUltimateAIController::DetectPlayer(APawn* PlayerPawn) const
{
	if (!PlayerPawn || !PerceptionComp) return false;
	
	// Check if player is in sight
	TArray<AActor*> PerceivedActors;
	PerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	
	return PerceivedActors.Contains(PlayerPawn);
}

void AUltimateAIController::SetLastKnownPlayerLocation(const FVector& Location)
{
	LastKnownPlayerLocation = Location;
	bHasKnownPlayerLocation = true;
	
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector("TargetLocation", Location);
	}
}

void AUltimateAIController::ExecuteGOAPPlan()
{
	// Execute the planned sequence of actions
	// This would typically involve queuing actions and executing them step by step
	
	if (CurrentGOAPPlan.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Executing GOAP plan with %d actions"), CurrentGOAPPlan.Num());
		// Execute first action in plan
	}
}

void AUltimateAIController::UpdateUtilityScores()
{
	// Recalculate utility scores for all available actions
	// Based on current world state, distance to targets, resources, etc.
}
