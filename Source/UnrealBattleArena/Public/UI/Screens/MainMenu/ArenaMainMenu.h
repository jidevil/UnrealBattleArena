// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ArenaScreen.h"
#include "ArenaMainMenu.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaMainMenu : public UArenaScreen
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	virtual void OnMenuItemClicked(const FName& MenuType);

	void OnPlayGame();
	void OnOptions();
	void OnExit();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Main Menu")
	TSubclassOf<class UArenaPlayGame> PlayGameClass;

	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* MenuVerticalBox{ nullptr };
};
