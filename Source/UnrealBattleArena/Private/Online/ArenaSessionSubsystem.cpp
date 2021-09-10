// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/ArenaSessionSubsystem.h"
#include "OnlineSubsystemUtils.h"

UArenaSessionSubsystem::UArenaSessionSubsystem()
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this,
		&UArenaSessionSubsystem::OnCreateSessionCompleted);
	OnUpdateSessionCompleteDelegate = FOnUpdateSessionCompleteDelegate::CreateUObject(this,
		&UArenaSessionSubsystem::OnUpdateSessionCompleted);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this,
		&UArenaSessionSubsystem::OnStartSessionCompleted);
	OnEndSessionCompleteDelegate = FOnEndSessionCompleteDelegate::CreateUObject(this,
		&UArenaSessionSubsystem::OnEndSessionCompleted);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this,
		&UArenaSessionSubsystem::OnDestroySessionCompleted);
	OnFindSessionCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this,
		&UArenaSessionSubsystem::OnFindSessionsCompleted);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this,
		&UArenaSessionSubsystem::OnJoinSessionCompleted);
}

void UArenaSessionSubsystem::CreateSession(int32 NumPublicConnections, bool bIsLANMatch,
	const TMap<FName, FString>& Settings)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		CreateSessionCompleteDelegate.Broadcast(false);
		return;
	}

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = bIsLANMatch;
	LastSessionSettings->bShouldAdvertise = true;

	for (const TPair<FName, FString>& SettingPair : Settings)
	{
		LastSessionSettings->Set(SettingPair.Key, SettingPair.Value,
			EOnlineDataAdvertisementType::ViaOnlineService);		
	}

	OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
		OnCreateSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(),
		NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
		CreateSessionCompleteDelegate.Broadcast(false);
	}
}

void UArenaSessionSubsystem::UpdateSession(const TMap<FName, FString>& Settings)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		UpdateSessionCompleteDelegate.Broadcast(false);
		return;
	}

	const TSharedPtr<FOnlineSessionSettings> UpdatedSessionSettings = MakeShareable(
		new FOnlineSessionSettings(*LastSessionSettings));

	for (const TPair<FName, FString>& SettingPair : Settings)
	{
		UpdatedSessionSettings->Set(SettingPair.Key, SettingPair.Value,
			EOnlineDataAdvertisementType::ViaOnlineService);		
	}

	OnUpdateSessionCompleteDelegateHandle = SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(
		OnUpdateSessionCompleteDelegate);

	if (!SessionInterface->UpdateSession(NAME_GameSession, *UpdatedSessionSettings))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
		UpdateSessionCompleteDelegate.Broadcast(false);
	}
	else
	{
		LastSessionSettings = UpdatedSessionSettings;
	}
}

void UArenaSessionSubsystem::StartSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		StartSessionCompleteDelegate.Broadcast(false);
		return;
	}

	OnStartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(
		OnStartSessionCompleteDelegate);

	if (!SessionInterface->StartSession(NAME_GameSession))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		StartSessionCompleteDelegate.Broadcast(false);
	}
}

void UArenaSessionSubsystem::EndSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		EndSessionCompleteDelegate.Broadcast(false);
		return;
	}

	OnEndSessionCompleteDelegateHandle = SessionInterface->AddOnEndSessionCompleteDelegate_Handle(
		OnEndSessionCompleteDelegate);

	if (!SessionInterface->EndSession(NAME_GameSession))
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegateHandle);
		EndSessionCompleteDelegate.Broadcast(false);
	}
}

void UArenaSessionSubsystem::DestroySession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		DestroySessionCompleteDelegate.Broadcast(false);
		return;
	}

	OnDestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
		OnDestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(NAME_GameSession))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		DestroySessionCompleteDelegate.Broadcast(false);
	}
}

void UArenaSessionSubsystem::FindSessions(int32 MaxSearchResults, bool bIsLANQuery)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		FindSessionCompleteDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	OnFindSessionCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
		OnFindSessionCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = bIsLANQuery;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(),
		LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompleteDelegateHandle);
		FindSessionCompleteDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UArenaSessionSubsystem::JoinGameSession(const FOnlineSessionSearchResult& SessionResult)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		JoinSessionCompleteDelegate.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
		OnJoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
		JoinSessionCompleteDelegate.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UArenaSessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool bSuccessful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	}

	CreateSessionCompleteDelegate.Broadcast(bSuccessful);
}

void UArenaSessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool bSuccessful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
	}

	UpdateSessionCompleteDelegate.Broadcast(bSuccessful);
}

void UArenaSessionSubsystem::OnStartSessionCompleted(FName SessionName, bool bSuccessful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
	}

	StartSessionCompleteDelegate.Broadcast(bSuccessful);
}

void UArenaSessionSubsystem::OnEndSessionCompleted(FName SessionName, bool bSuccessful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegateHandle);
	}

	EndSessionCompleteDelegate.Broadcast(bSuccessful);
}

void UArenaSessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool bSuccessful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	}

	DestroySessionCompleteDelegate.Broadcast(bSuccessful);
}

void UArenaSessionSubsystem::OnFindSessionsCompleted(bool bSuccessful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		FindSessionCompleteDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	FindSessionCompleteDelegate.Broadcast(LastSessionSearch->SearchResults, bSuccessful);
}

void UArenaSessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	}

	JoinSessionCompleteDelegate.Broadcast(Result);
}

bool UArenaSessionSubsystem::TravelToCurrentSession() const
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	FString ConnectString;
	if (!SessionInterface->GetResolvedConnectString(NAME_GameSession, ConnectString))
	{
		return false;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return false;
	}

	PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
	return true;
}

bool UArenaSessionSubsystem::GetServerTravelURL(FString& TravelURL) const
{
	FString MapName;
	LastSessionSettings->Get(SETTING_MAPNAME, MapName);
	TravelURL = FString::Printf(TEXT("/Game/UnrealBattleArena/Maps/%s?listen"), *MapName);
	
	return true;
}
