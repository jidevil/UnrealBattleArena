// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ArenaUIManager.h"
#include "UI/ArenaScreen.h"

UArenaDialog* UArenaUIManager::ShowDialog(EDialogType DialogType, FDialogOnButtonClickDelegate OnButtonClickDelegate,
                                          TSubclassOf<UArenaDialog> DialogClass)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		UArenaDialog* Dialog = Cast<UArenaDialog>(CreateWidget(PlayerController, DialogClass ?
			DialogClass : DefaultDialogClass));

		if (Dialog)
		{
			Dialog->OnButtonClick.BindLambda([Dialog, OnButtonClickDelegate](EDialogButtonType ButtonType)
			{
				if (OnButtonClickDelegate.IsBound())
				{
					OnButtonClickDelegate.Execute(ButtonType);
				}

				Dialog->RemoveFromViewport();
				Dialog->Destruct();
			});

			Dialog->SetType(DialogType);
			Dialog->AddToViewport();

			return Dialog;
		}
	}

	return nullptr;
}

void UArenaUIManager::PushScreen(TSubclassOf<UArenaScreen> ScreenClass)
{
	if (HasScreen(ScreenClass))
	{
		PopToFront(ScreenClass);
	}
	else
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			UArenaScreen* Screen = Cast<UArenaScreen>(CreateWidget(PlayerController, ScreenClass));
			if (Screen)
			{
				if (Screens.Num() > 0)
				{
					Screens.Top()->RemoveFromViewport();
				}

				Screen->AddToViewport();
				Screens.Push(Screen);
			}
		}
	}
}

void UArenaUIManager::PopScreen()
{
	if (Screens.Num() > 0)
	{
		Screens.Top()->RemoveFromViewport();
		Screens.Top()->Destruct();
		Screens.RemoveAt(Screens.Num() - 1);

		if (Screens.Num() > 0)
		{
			Screens.Top()->AddToViewport();
		}
	}
}

void UArenaUIManager::PushToBack(TSubclassOf<UArenaScreen> ScreenClass)
{
	if (Screens.Num() > 1)
	{
		int32 ScreenIndex = -1;
		for (int32 Index = 0; Index < Screens.Num(); Index++)
		{
			if (Screens[Index]->GetClass() == ScreenClass)
			{
				ScreenIndex = Index;
				break;
			}
		}

		if (ScreenIndex > 0)
		{			
			UArenaScreen* Screen = Screens[ScreenIndex];
			if (ScreenIndex == Screens.Num() - 1)
			{
				Screen->RemoveFromViewport();	
			}
			
			for (int32 Index = ScreenIndex - 1; Index >= 0; Index--)
			{
				Screens[Index + 1] = Screens[Index];
			}

			if (ScreenIndex == Screens.Num() - 1)
			{
				Screens.Top()->AddToViewport();	
			}

			Screens[0] = Screen;
		}
	}
}

void UArenaUIManager::PopToFront(TSubclassOf<UArenaScreen> ScreenClass)
{
	if (Screens.Num() > 1)
	{
		int32 ScreenIndex = -1;
		for (int32 Index = 0; Index < Screens.Num(); Index++)
		{
			if (Screens[Index]->GetClass() == ScreenClass)
			{
				ScreenIndex = Index;
				break;
			}
		}

		if (ScreenIndex != -1 && ScreenIndex != Screens.Num() - 1)
		{
			Screens.Top()->RemoveFromViewport();
			
			UArenaScreen* Screen = Screens[ScreenIndex];
			for (int32 Index = ScreenIndex + 1; Index < Screens.Num(); Index++)
			{
				Screens[Index - 1] = Screens[Index];
			}

			Screens[Screens.Num() - 1] = Screen;
			Screens.Top()->AddToViewport();
		}
	}
}

bool UArenaUIManager::HasScreen(TSubclassOf<UArenaScreen> ScreenClass)
{
	for (const UArenaScreen* Screen : Screens)
	{
		if (Screen->GetClass() == ScreenClass)
		{
			return true;
		}
	}

	return false;
}
