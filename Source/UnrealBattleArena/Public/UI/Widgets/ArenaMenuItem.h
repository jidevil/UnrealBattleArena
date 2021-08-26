// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaMenuItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMenuItemClickDelegate, const FName&, MenuType);

UCLASS()
class UNREALBATTLEARENA_API UArenaMenuItem : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetText(const FText& Text);

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnMenuItemClicked();

public:

	FMenuItemClickDelegate OnMenuItemClick;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Arena Menu Item")
	FName MenuType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Arena Menu Item")
	FText MenuTitle;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MenuText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* MenuButton{ nullptr };
};
