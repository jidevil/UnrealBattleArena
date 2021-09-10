// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaPlayGameItem.generated.h"

UCLASS()
class UNREALBATTLEARENA_API UArenaPlayGameItem : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnPlayButtonClicked();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Arena Play Game Item")
	FText GameType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Arena Play Game Item")
	TSubclassOf<class UArenaChooseGame> ChooseGameClass;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* GameTypeText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* PlayButton{ nullptr };
};
