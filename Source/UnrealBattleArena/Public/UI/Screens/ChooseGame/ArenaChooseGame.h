// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ArenaScreen.h"
#include "ArenaChooseGame.generated.h"



UCLASS()
class UNREALBATTLEARENA_API UArenaChooseGame : public UArenaScreen
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void OnFindSessionsCompleted(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bSuccessful);
	
	UFUNCTION()
	void OnCreateButtonClicked();

	UFUNCTION()
	void OnRefreshButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();
		
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Choose Game")
	TSubclassOf<class UArenaChooseGameItem> GameItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Choose Game")
	TSubclassOf<class UArenaCreateGame> CreateGameClass;

	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* GameItemListBox{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* CreateButton{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* RefreshButton{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* BackButton{ nullptr };
};
