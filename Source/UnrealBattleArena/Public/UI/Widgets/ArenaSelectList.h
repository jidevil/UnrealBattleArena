// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaSelectList.generated.h"

USTRUCT(BlueprintType)
struct FSelectItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Value;
};

UCLASS()
class UNREALBATTLEARENA_API UArenaSelectList : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	const FSelectItem& GetSelectedItem() const;

	UFUNCTION(BlueprintCallable)
	void AddItem(const FText& Title, const FString& Value);

	UFUNCTION(BlueprintCallable)
	void SetCurrentItem(int32 Index);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnPreviousButtonClicked();

	UFUNCTION()
	void OnNextButtonClicked();

protected:

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ContentText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* PreviousButton{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* NextButton{ nullptr };

	TArray<FSelectItem> Items;
	int32 CurrentIndex{ -1 };
};
