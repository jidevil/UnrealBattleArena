// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ArenaSessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateSessionCompleteDelegate, bool, bSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSessionCompleteDelegate, bool, bSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartSessionCompleteDelegate, bool, bSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndSessionCompleteDelegate, bool, bSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroySessionCompleteDelegate, bool, bSuccessful);

DECLARE_MULTICAST_DELEGATE_TwoParams(FFindSessionCompleteDelegate,
	const TArray<FOnlineSessionSearchResult>& SessionResults, bool bSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FJoinSessionCompleteDelegate,
	EOnJoinSessionCompleteResult::Type Result);

UCLASS()
class UNREALBATTLEARENA_API UArenaSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UArenaSessionSubsystem();

	void CreateSession(int32 NumPublicConnections, bool bIsLANMatch, const FString& MapName);
	void UpdateSession(const FString& MapName);
	void StartSession();
	void EndSession();
	void DestroySession();
	void FindSessions(int32 MaxSearchResults, bool bIsLANQuery);
	void JoinGameSession(const FOnlineSessionSearchResult& SessionResult);

public:

	FCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FFindSessionCompleteDelegate FindSessionCompleteDelegate;
	FJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

protected:

	void OnCreateSessionCompleted(FName SessionName, bool bSuccessful);
	void OnUpdateSessionCompleted(FName SessionName, bool bSuccessful);
	void OnStartSessionCompleted(FName SessionName, bool bSuccessful);
	void OnEndSessionCompleted(FName SessionName, bool bSuccessful);
	void OnDestroySessionCompleted(FName SessionName, bool bSuccessful);
	void OnFindSessionsCompleted(bool bSuccessful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession() const;

protected:

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnUpdateSessionCompleteDelegate OnUpdateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnEndSessionCompleteDelegate OnEndSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnUpdateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnEndSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
};
