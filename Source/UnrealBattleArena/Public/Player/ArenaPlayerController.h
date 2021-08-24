// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArenaPlayerController.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	FORCEINLINE class UArenaHUD* GetPlayerHUD() const { return PlayerHUD; }

protected:

	virtual void BeginPlay() override;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Player Controller")
	TSubclassOf<class UArenaHUD> PlayerHUDClass;

	UPROPERTY(Transient)
	class UArenaHUD* PlayerHUD{ nullptr };
};
