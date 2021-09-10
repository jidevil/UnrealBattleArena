// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ArenaChooseGameItem.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaChooseGameItem : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetSessionSearchResult(FOnlineSessionSearchResult InSessionSearchResult);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void OnJoinSessionCompleted(EOnJoinSessionCompleteResult::Type Result);
	FString GetSetting(const FName& Key);

	UFUNCTION()
	void OnJoinButtonClicked();
		
protected:

	FOnlineSessionSearchResult SessionSearchResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxPlayers{ 0 };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* GameNameText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CreatedByText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MapNameText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PlayersText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* JoinButton{ nullptr };
};
