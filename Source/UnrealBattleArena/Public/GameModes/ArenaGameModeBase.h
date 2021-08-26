// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameModeBase.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	FORCEINLINE class UArenaUIManager* GetUIManager() const { return UIManager; }

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena GameMode Base")
	TSubclassOf<class UArenaUIManager> UIManagerClass;

	UPROPERTY(Transient)
	class UArenaUIManager* UIManager{ nullptr };
};
