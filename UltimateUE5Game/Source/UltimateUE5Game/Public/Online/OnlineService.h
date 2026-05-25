// Copyright 2024 UltimateUE5Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineService.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLeaderboardUpdated, const FString&, LeaderboardName, const TArray<int32>&, Scores);

/**
 * Online Service Subsystem
 * Handles leaderboards, friends, sessions, cloud saves, and multiplayer
 */
UCLASS()
class ULTIMATEUE5GAME_API UOnlineService : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void Initialize(class UUltimateGameInstance* InGameInstance);

	// Leaderboards
	UFUNCTION(BlueprintCallable, Category = "Online|Leaderboards")
	void SubmitScore(const FString& LeaderboardName, int32 Score);

	UFUNCTION(BlueprintCallable, Category = "Online|Leaderboards")
	void GetLeaderboard(const FString& LeaderboardName, int32 MaxEntries = 10);

	UPROPERTY(BlueprintAssignable, Category = "Online|Leaderboards")
	FOnLeaderboardUpdated OnLeaderboardUpdated;

	// Friends System
	UFUNCTION(BlueprintCallable, Category = "Online|Friends")
	void GetFriendsList();

	UFUNCTION(BlueprintCallable, Category = "Online|Friends")
	void InviteFriend(const FString& FriendName);

	UFUNCTION(BlueprintCallable, Category = "Online|Friends")
	void JoinFriendSession(const FString& FriendName);

	// Sessions & Multiplayer
	UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
	void CreateSession(int32 MaxPlayers, bool bIsPublic);

	UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
	void FindSessions();

	UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
	void JoinSession(int32 SessionIndex);

	UFUNCTION(BlueprintCallable, Category = "Online|Sessions")
	void DestroySession();

	// Cloud Saves
	UFUNCTION(BlueprintCallable, Category = "Online|Cloud")
	void SaveCloudData();

	UFUNCTION(BlueprintCallable, Category = "Online|Cloud")
	void LoadCloudData();

	// Achievements
	UFUNCTION(BlueprintCallable, Category = "Online|Achievements")
	void UnlockAchievement(const FString& AchievementID);

protected:
	UPROPERTY()
	TObjectPtr<class UUltimateGameInstance> GameInstance;

private:
	bool bIsInitialized;
	
	void OnSubmitScoreComplete(bool bWasSuccessful);
	void OnGetLeaderboardComplete(bool bWasSuccessful, const TArray<int32>& Scores);
	void OnFindSessionsComplete(bool bWasSuccessful);
};
