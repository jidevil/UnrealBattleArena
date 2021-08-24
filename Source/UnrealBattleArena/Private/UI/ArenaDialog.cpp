// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ArenaDialog.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UArenaDialog::SetType(EDialogType InDialogType)
{
	switch (InDialogType)
	{
	case EDialogType::OkOnly:
		{
			YesButton->SetVisibility(ESlateVisibility::Visible);
			NoButton->SetVisibility(ESlateVisibility::Hidden);
			CancelButton->SetVisibility(ESlateVisibility::Hidden);
			break;
		}

	case EDialogType::YesNo:
		{
			YesButton->SetVisibility(ESlateVisibility::Visible);
			NoButton->SetVisibility(ESlateVisibility::Visible);
			CancelButton->SetVisibility(ESlateVisibility::Hidden);
			break;
		}

	case EDialogType::YesNoCancel:
		{
			YesButton->SetVisibility(ESlateVisibility::Visible);
			NoButton->SetVisibility(ESlateVisibility::Visible);
			CancelButton->SetVisibility(ESlateVisibility::Visible);
			break;
		}
	}
}

void UArenaDialog::SetText(EDialogButtonType ButtonType, const FText& Text)
{
	switch (ButtonType)
	{
	case EDialogButtonType::Yes:
		{
			YesButtonText->SetText(Text);
			break;
		}

	case EDialogButtonType::No:
		{
			NoButtonText->SetText(Text);
			break;
		}

	case EDialogButtonType::Cancel:
		{
			CancelButtonText->SetText(Text);
			break;
		}
	}
}

void UArenaDialog::NativeConstruct()
{
	Super::NativeConstruct();

	YesButton->OnClicked.AddDynamic(this, &UArenaDialog::OnYesClicked);
	NoButton->OnClicked.AddDynamic(this, &UArenaDialog::OnNoClicked);
	CancelButton->OnClicked.AddDynamic(this, &UArenaDialog::OnCancelClicked);
}

void UArenaDialog::OnYesClicked()
{
	if (OnButtonClick.IsBound())
	{
		OnButtonClick.Execute(EDialogButtonType::Yes);
	}
}

void UArenaDialog::OnNoClicked()
{
	if (OnButtonClick.IsBound())
	{
		OnButtonClick.Execute(EDialogButtonType::No);
	}
}

void UArenaDialog::OnCancelClicked()
{
	if (OnButtonClick.IsBound())
	{
		OnButtonClick.Execute(EDialogButtonType::Cancel);
	}
}
