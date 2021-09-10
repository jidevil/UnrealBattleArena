// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaOnlineMatch.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/OnlineReplStructs.h"
#include "ArenaGameState.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	FORCEINLINE class UArenaOnlineMatch* GetOnlineMatch() const { return OnlineMatch; }

	void CreateMatch(const FString& GameName, const FString& MapName, int32 TotalPlayers);
	void JoinMatch(const FString& PlayerName, bool bMatchOwner = false);

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Game State")
	class UArenaOnlineMatch* OnlineMatch{ nullptr };
};
