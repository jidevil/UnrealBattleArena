// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealBattleArena.h"
#include "UObject/NoExportTypes.h"
#include "ArenaOnlineMatch.generated.h"

USTRUCT(BlueprintType)
struct FOnlinePlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bMatchOwner{ false };
};

USTRUCT(BlueprintType)
struct FOnlineMatchInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MatchName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MapName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 TotalPlayers{ 0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EGameType GameType{ EGameType::Coop };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FOnlinePlayerInfo> Players;
};

UCLASS()
class UNREALBATTLEARENA_API UArenaOnlineMatch : public UObject
{
	GENERATED_BODY()

public:

	FORCEINLINE const FOnlineMatchInfo& GetMatchInfo() const { return MatchInfo; }

	void NewMatch(const FString& MatchName, const FString& MapName,
		int32 TotalPlayers, EGameType GameType);
	
	void JoinPlayer(const FOnlinePlayerInfo& Player);
	bool CanStartMatch();
	void StartMatch();	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Online Match")
	FOnlineMatchInfo MatchInfo;
};
