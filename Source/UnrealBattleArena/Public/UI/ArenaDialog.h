// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArenaDialog.generated.h"

UENUM(BlueprintType)
enum class EDialogType : uint8
{
	OkOnly UMETA(DisplayName="OkOnly"),
	YesNo UMETA(DisplayName="YesNo"),
	YesNoCancel UMETA(DisplayName="YesNoCancel")
};

UENUM(BlueprintType)
enum class EDialogButtonType : uint8
{
	Yes UMETA(DisplayName="Yes"),
	No UMETA(DisplayName="No"),
	Cancel UMETA(DisplayName="Cancel")
};

DECLARE_DELEGATE_OneParam(FDialogOnButtonClickEvent, EDialogButtonType);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDialogOnButtonClickDelegate, EDialogButtonType, ButtonType);

UCLASS()
class UNREALBATTLEARENA_API UArenaDialog : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetType(EDialogType InDialogType);
	void SetText(EDialogButtonType ButtonType, const FText& Text);

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnYesClicked();

	UFUNCTION()
	void OnNoClicked();

	UFUNCTION()
	void OnCancelClicked();

public:

	FDialogOnButtonClickEvent OnButtonClick;

protected:
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ContentText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* YesButtonText{ nullptr };
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NoButtonText{ nullptr };
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CancelButtonText{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* YesButton{ nullptr };
	
	UPROPERTY(meta=(BindWidget))
	class UButton* NoButton{ nullptr };

	UPROPERTY(meta=(BindWidget))
	class UButton* CancelButton{ nullptr };
};
