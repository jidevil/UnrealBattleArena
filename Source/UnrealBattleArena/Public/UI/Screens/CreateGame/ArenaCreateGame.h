// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ArenaScreen.h"
#include "ArenaCreateGame.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaCreateGame : public UArenaScreen
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	FString GetOptionsString();
	
	UFUNCTION()
	void OnCreateSessionCompleted(bool bSuccessful);

	UFUNCTION()
	void OnCreateButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

protected:

	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* GameNameEditText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UArenaSelectList* MapNameSelectList{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UArenaSelectList* PlayersSelectList{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* CreateButton{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* CancelButton{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* BackButton{ nullptr };

	int32 NumPublicConnections{ 10 };
	bool bIsLANMatch{ false };
};
