// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ArenaPlayerState.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	AArenaPlayerState();

	FORCEINLINE int32 GetTeamNumber() const { return TeamNumber; }
	FORCEINLINE bool IsBot() const { return bIsBot; }
	
	void SetTeamNumber(int32 InTeamNumber);
	void SetIsBot(bool bInIsBot);

protected:

	UPROPERTY(Replicated, Transient, BlueprintReadOnly, Category="Arena Player State")
	int32 TeamNumber{ 0 };

	UPROPERTY(Replicated, Transient, BlueprintReadOnly, Category="Arena Player State")
	bool bIsBot{ false };
};
