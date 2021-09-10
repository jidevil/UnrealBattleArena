// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "ArenaPlayerStart.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:

	FORCEINLINE int32 GetTeamNumber() const { return TeamNumber; }
	
protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Arena Player Start")
	int32 TeamNumber{ 0 };
};
