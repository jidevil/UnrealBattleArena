// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ArenaScreen.h"
#include "ArenaPlayGame.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaPlayGame : public UArenaScreen
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnBackButtonClicked();

protected:

	UPROPERTY(meta=(BindWidget))
	class UArenaPlayGameItem* CoopGameItem{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UArenaPlayGameItem* VersusGameItem{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UArenaPlayGameItem* TeamDeathMatchGameItem{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* BackButton{ nullptr };
};
