// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#include "AI/GOAPSystem.h"
#include "GameFramework/Pawn.h"

bool UGOAPAction::CanExecute(const TMap<FString, float>& WorldState) const
{
	// Check if all preconditions are met
	for (auto& Precondition : Preconditions)
	{
		if (!WorldState.Contains(Precondition.Key))
		{
			return false;
		}
		
		if (WorldState[Precondition.Key] < Precondition.Value)
		{
			return false;
		}
	}
	
	return true;
}

void UGOAPAction::Execute(APawn* Executor)
{
	if (!Executor) return;
	
	// Execute the action logic
	// This would typically trigger animations, move the AI, use items, etc.
	UE_LOG(LogTemp, Log, TEXT("Executing GOAP Action: %s"), *ActionName);
}

bool UGOAPGoal::IsSatisfied(const TMap<FString, float>& WorldState) const
{
	// Check if the goal state is achieved
	for (auto& Desired : DesiredState)
	{
		if (!WorldState.Contains(Desired.Key))
		{
			return false;
		}
		
		if (WorldState[Desired.Key] < Desired.Value)
		{
			return false;
		}
	}
	
	return true;
}

TArray<FString> UGOAPSystem::PlanActions(
	const TArray<UGOAPAction*>& AvailableActions,
	const TMap<FString, float>& CurrentState,
	const TMap<FString, float>& GoalState)
{
	TArray<FString> Plan;
	
	if (FindPath(AvailableActions, CurrentState, GoalState, Plan, 0))
	{
		UE_LOG(LogTemp, Log, TEXT("GOAP Plan found with %d actions"), Plan.Num());
		return Plan;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("No valid GOAP plan found"));
	return TArray<FString>();
}

bool UGOAPSystem::IsValidPlan(const TArray<FString>& Plan, const TArray<UGOAPAction*>& Actions)
{
	if (Plan.Num() == 0) return false;
	
	// Validate that each action in the plan exists and can be chained
	TMap<FString, UGOAPAction*> ActionMap;
	for (UGOAPAction* Action : Actions)
	{
		ActionMap.Add(Action->ActionName, Action);
	}
	
	TMap<FString, float> SimulatedState;
	
	// Verify preconditions and effects chain correctly
	for (const FString& ActionName : Plan)
	{
		if (!ActionMap.Contains(ActionName))
		{
			return false;
		}
		
		UGOAPAction* Action = ActionMap[ActionName];
		if (!Action->CanExecute(SimulatedState))
		{
			return false;
		}
		
		// Apply effects to simulated state
		for (auto& Effect : Action->Effects)
		{
			SimulatedState.Add(Effect.Key, Effect.Value);
		}
	}
	
	return true;
}

bool UGOAPSystem::FindPath(
	const TArray<UGOAPAction*>& Actions,
	TMap<FString, float> CurrentState,
	const TMap<FString, float>& GoalState,
	TArray<FString>& OutPlan,
	int32 Depth)
{
	// Simple depth-limited search for planning
	const int32 MaxDepth = 10;
	
	if (Depth > MaxDepth)
	{
		return false;
	}
	
	// Check if goal is satisfied
	bool bGoalSatisfied = true;
	for (auto& Goal : GoalState)
	{
		if (!CurrentState.Contains(Goal.Key) || CurrentState[Goal.Key] < Goal.Value)
		{
			bGoalSatisfied = false;
			break;
		}
	}
	
	if (bGoalSatisfied)
	{
		return true;
	}
	
	// Try each action
	for (UGOAPAction* Action : Actions)
	{
		if (Action->CanExecute(CurrentState))
		{
			// Apply effects
			TMap<FString, float> NewState = CurrentState;
			for (auto& Effect : Action->Effects)
			{
				NewState.Add(Effect.Key, Effect.Value);
			}
			
			OutPlan.Add(Action->ActionName);
			
			if (FindPath(Actions, NewState, GoalState, OutPlan, Depth + 1))
			{
				return true;
			}
			
			OutPlan.RemoveAt(OutPlan.Num() - 1);
		}
	}
	
	return false;
}
