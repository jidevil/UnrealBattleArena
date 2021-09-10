// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArenaFunctionLibrary.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static class AArenaGameModeBase* GetGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static class AArenaGameState* GetGameState(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static class AArenaPlayerController* GetPlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static class UArenaSessionSubsystem* GetSessionSubsystem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static class UArenaUIManager* GetUIManager(const UObject* WorldContextObject);
};
