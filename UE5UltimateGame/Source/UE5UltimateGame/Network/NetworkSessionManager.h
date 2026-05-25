#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UNetworkSessionManager.generated.h"

// Состояния сессии
UENUM(BlueprintType)
enum class ESessionState : uint8
{
    NoSession,
    Creating,
    Created,
    Joining,
    Joined,
    Destroying,
    Searching,
    FoundSessions
};

// Структура данных о найденной сессии
USTRUCT(BlueprintType)
struct FSessionSearchResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FString SessionName;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FString HostName;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 CurrentPlayers;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 MaxPlayers;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    bool bIsPrivate;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FString MapName;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 Ping;

    FSessionSearchResult() 
        : CurrentPlayers(0), MaxPlayers(0), bIsPrivate(false), Ping(0) {}
};

// Делегаты для событий сети
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionStateChanged, ESessionState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSessionFound, const FSessionSearchResult&, Result, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionJoined, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionCreated, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerJoined, APlayerController*, Player, const FString&, PlayerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerLeft, APlayerController*, Player, const FString&, PlayerName);

UCLASS()
class UE5ULTIMATEGAME_API UNetworkSessionManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UNetworkSessionManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // === УПРАВЛЕНИЕ СЕССИЯМИ ===
    
    // Создать новую сессию
    UFUNCTION(BlueprintCallable, Category = "Network|Session")
    void CreateSession(int32 MaxPlayers, bool bIsPrivate, FString CustomServerName);

    // Присоединиться к сессии по имени
    UFUNCTION(BlueprintCallable, Category = "Network|Session")
    void JoinSession(FString SessionName);

    // Присоединиться к сессии по индексу
    UFUNCTION(BlueprintCallable, Category = "Network|Session")
    void JoinSessionByIndex(int32 SessionIndex);

    // Уничтожить текущую сессию
    UFUNCTION(BlueprintCallable, Category = "Network|Session")
    void DestroySession();

    // Найти сессии
    UFUNCTION(BlueprintCallable, Category = "Network|Session")
    void FindSessions(int32 MaxResults, bool bUseLAN);

    // Обновить настройки сессии
    UFUNCTION(BlueprintCallable, Category = "Network|Session")
    void UpdateSessionSettings(int32 NewMaxPlayers, FString NewMapName);

    // === ИГРОКИ ===
    
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Players")
    int32 GetNumPlayers() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Players")
    int32 GetMaxPlayers() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Players")
    TArray<APlayerController*> GetAllPlayers();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Players")
    FString GetPlayerName(APlayerController* Player) const;

    // === СЕРВЕР/КЛИЕНТ ===
    
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Role")
    bool IsServer() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Role")
    bool IsClient() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Role")
    bool HasAuthority() const;

    // Travel на сервер
    UFUNCTION(BlueprintCallable, Category = "Network|Travel")
    void ServerTravel(FString MapName, bool bAbsolute = false);

    UFUNCTION(BlueprintCallable, Category = "Network|Travel")
    void ClientTravel(FString URL);

    // === СТАТУС ===
    
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Status")
    ESessionState GetCurrentSessionState() const { return SessionState; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Status")
    FString GetCurrentSessionName() const { return CurrentSessionName; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network|Status")
    TArray<FSessionSearchResult> GetFoundSessions() const { return FoundSessions; }

    // События
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSessionStateChanged OnSessionStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSessionFound OnSessionFound;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSessionJoined OnSessionJoined;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSessionCreated OnSessionCreated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerJoined OnPlayerJoinedDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerLeft OnPlayerLeftDelegate;

private:
    ESessionState SessionState;
    FString CurrentSessionName;
    TArray<FSessionSearchResult> FoundSessions;
    
    TSharedPtr<FOnlineSessionSearch> SessionSearch;
    
    // Callbacks
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
    void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);

    void BroadcastSessionState(ESessionState NewState);
};
