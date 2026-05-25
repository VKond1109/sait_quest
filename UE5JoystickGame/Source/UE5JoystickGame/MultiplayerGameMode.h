// Copyright 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerGameMode.generated.h"

UCLASS()
class UE5JOYSTICKGAME_API AMultiplayerGameMode : public AGameMode
{
GENERATED_BODY()

public:
AMultiplayerGameMode();

// Создать новую сессию
UFUNCTION(BlueprintCallable, Category = "Multiplayer")
void CreateServerSession(FName SessionName, int32 MaxPlayers);

// Найти сессии
UFUNCTION(BlueprintCallable, Category = "Multiplayer")
void FindSessions();

// Присоединиться к сессии
UFUNCTION(BlueprintCallable, Category = "Multiplayer")
void JoinSession(int32 SessionIndex);

// Уничтожить сессию
UFUNCTION(BlueprintCallable, Category = "Multiplayer")
void DestroySession();

// Событие успешного создания сессии
UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer|Events")
void OnSessionCreatedSuccess();

// Событие ошибки создания сессии
UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer|Events")
void OnSessionCreateFailed();

// Событие найденных сессий
UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer|Events")
void OnSessionsFound(const TArray<FString>& SessionNames, const TArray<int32>& PlayerCounts, const TArray<int32>& MaxPlayers);

// Событие успешного присоединения
UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer|Events")
void OnJoinSessionSuccess();

// Событие ошибки присоединения
UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer|Events")
void OnJoinSessionFailed();

protected:
// Обработчики событий онлайн подсистемы
FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;

FDelegateHandle CreateSessionCompleteDelegateHandle;
FDelegateHandle FindSessionsCompleteDelegateHandle;
FDelegateHandle JoinSessionCompleteDelegateHandle;
FDelegateHandle DestroySessionCompleteDelegateHandle;

// Массив результатов поиска
TArray<FOnlineSessionSearchResult> SessionSearchResults;

// Обработчики
void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
void OnFindSessionsComplete(bool bWasSuccessful);
void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};
