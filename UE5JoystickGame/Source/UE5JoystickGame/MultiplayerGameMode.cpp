// Copyright 2024. All rights reserved.

#include "MultiplayerGameMode.h"
#include "Engine/LocalPlayer.h"
#include "Online/SubsystemUtils.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"

AMultiplayerGameMode::AMultiplayerGameMode()
{
CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AMultiplayerGameMode::OnCreateSessionComplete);
FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AMultiplayerGameMode::OnFindSessionsComplete);
JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AMultiplayerGameMode::OnJoinSessionComplete);
DestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &AMultiplayerGameMode::OnDestroySessionComplete);
}

void AMultiplayerGameMode::CreateServerSession(FName SessionName, int32 MaxPlayers)
{
IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
if (!SessionInterface.IsValid())
{
OnSessionCreateFailed();
return;
}

CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

FOnlineSessionSettings SessionSettings;
SessionSettings.NumPublicConnections = MaxPlayers;
SessionSettings.bShouldAdvertise = true;
SessionSettings.bAllowJoinInProgress = true;
SessionSettings.bIsLANMatch = false;
SessionSettings.bUsesPresence = true;
SessionSettings.bAllowJoinViaPresence = true;
SessionSettings.Set(SEARCH_KEYWORDS, SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
if (!SessionInterface->CreateSession(LocalPlayer ? LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId() : 0, SessionName, SessionSettings))
{
SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
OnSessionCreateFailed();
}
}

void AMultiplayerGameMode::FindSessions()
{
IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
if (!SessionInterface.IsValid())
{
return;
}

FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

SessionSearch = MakeShareable(new FOnlineSessionSearch);
SessionSearch->MaxSearchResults = 100;
SessionSearch->bIsLanQuery = false;
SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
if (!SessionInterface->FindSessions(LocalPlayer ? LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId() : 0, SessionSearch.ToSharedRef()))
{
SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
}
}

void AMultiplayerGameMode::JoinSession(int32 SessionIndex)
{
IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
if (!SessionInterface.IsValid() || SessionIndex >= SessionSearchResults.Num())
{
OnJoinSessionFailed();
return;
}

JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
if (!SessionInterface->JoinSession(LocalPlayer ? LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId() : 0, NAME_GameSession, SessionSearchResults[SessionIndex]))
{
SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
OnJoinSessionFailed();
}
}

void AMultiplayerGameMode::DestroySession()
{
IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
if (!SessionInterface.IsValid())
{
return;
}

DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
SessionInterface->DestroySession(NAME_GameSession);
}

void AMultiplayerGameMode::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
if (SessionInterface.IsValid())
{
SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
}

if (bWasSuccessful)
{
OnSessionCreatedSuccess();
}
else
{
OnSessionCreateFailed();
}
}

void AMultiplayerGameMode::OnFindSessionsComplete(bool bWasSuccessful)
{
IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
if (SessionInterface.IsValid())
{
SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
}

if (bWasSuccessful && SessionSearch.IsValid())
{
SessionSearchResults = SessionSearch->SearchResults;

TArray<FString> Names;
TArray<int32> Players;
TArray<int32> Maxs;

for (const auto& Result : SessionSearchResults)
{
FString SessionName;
Result.Session.SessionSettings.Get(SEARCH_KEYWORDS, SessionName);
Names.Add(SessionName);
Players.Add(Result.Session.NumOpenPrivateConnections + Result.Session.NumOpenPublicConnections);
Maxs.Add(Result.Session.SessionSettings.NumPublicConnections);
}

OnSessionsFound(Names, Players, Maxs);
}
}

void AMultiplayerGameMode::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
if (SessionInterface.IsValid())
{
SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
}

if (Result == EOnJoinSessionCompleteResult::Success)
{
OnJoinSessionSuccess();
}
else
{
OnJoinSessionFailed();
}
}

void AMultiplayerGameMode::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
if (SessionInterface.IsValid())
{
SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
}
}
