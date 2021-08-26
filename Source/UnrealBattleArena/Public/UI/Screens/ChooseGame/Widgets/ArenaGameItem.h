// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaGameItem.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaGameItem : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetGameName(const FText& GameName);
	void SetCreatedBy(const FText& CreatedBy);
	void SetMapName(const FText& MapName);
	void SetPlayers(const FText& Players);
	void SetSessionId(const FString& InSessionId);
	
protected:

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

	UPROPERTY(Transient)
	FString SessionId;
};
