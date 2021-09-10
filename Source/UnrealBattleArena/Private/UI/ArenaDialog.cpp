// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ArenaDialog.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

void UArenaDialog::SetType(EDialogType InDialogType)
{
	switch (InDialogType)
	{
	case EDialogType::Empty:
		{
			if (ButtonBox)
			{
				ButtonBox->RemoveFromParent();
			}
			break;
		}
	case EDialogType::OkOnly:
		{
			if (ButtonBox)
			{
				ButtonBox->RemoveChild(NoButton);
				ButtonBox->RemoveChild(CancelButton);
			}
			break;
		}

	case EDialogType::YesNo:
		{
			if (ButtonBox)
			{
				ButtonBox->RemoveChild(CancelButton);
			}
			break;
		}

	case EDialogType::YesNoCancel:
		break;
	}
}

void UArenaDialog::SetContentText(const FText& Content)
{
	if (ContentText)
	{
		ContentText->SetText(Content);
	}
}

void UArenaDialog::SetButtonText(EDialogButtonType ButtonType, const FText& Text)
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

void UArenaDialog::Show()
{
	if (!IsInViewport())
	{
		AddToViewport();
	}
}

void UArenaDialog::Close()
{
	if (IsInViewport())
	{
		RemoveFromViewport();
	}

	Destruct();
}

void UArenaDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (YesButton)
	{
		YesButton->OnClicked.AddDynamic(this, &UArenaDialog::OnYesClicked);
	}

	if (NoButton)
	{
		NoButton->OnClicked.AddDynamic(this, &UArenaDialog::OnNoClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UArenaDialog::OnCancelClicked);
	}
}

void UArenaDialog::NativeDestruct()
{
	Super::NativeDestruct();

	if (YesButton)
	{
		YesButton->OnClicked.RemoveDynamic(this, &UArenaDialog::OnYesClicked);
	}

	if (NoButton)
	{
		NoButton->OnClicked.RemoveDynamic(this, &UArenaDialog::OnNoClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.RemoveDynamic(this, &UArenaDialog::OnCancelClicked);
	}
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
