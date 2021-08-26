// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/ArenaSelectList.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

const FSelectItem& UArenaSelectList::GetSelectedItem() const
{
	return Items[CurrentIndex];
}

void UArenaSelectList::AddItem(const FText& Title, const FString& Value)
{
	Items.Add(FSelectItem{ Title, Value });
}

void UArenaSelectList::NativeConstruct()
{
	Super::NativeConstruct();

	if (PreviousButton)
	{
		PreviousButton->OnClicked.AddDynamic(this, &UArenaSelectList::OnPreviousButtonClicked);
	}

	if (NextButton)
	{
		NextButton->OnClicked.AddDynamic(this, &UArenaSelectList::OnNextButtonClicked);
	}
}

void UArenaSelectList::SetCurrentItem(int32 Index)
{
	if (Index >= 0 && Index < Items.Num())
	{
		CurrentIndex = Index;

		if (ContentText)
		{
			ContentText->SetText(Items[CurrentIndex].Title);
		}
	}
}

void UArenaSelectList::OnPreviousButtonClicked()
{
	if (CurrentIndex > 0)
	{
		CurrentIndex--;
		SetCurrentItem(CurrentIndex);		
	}
}

void UArenaSelectList::OnNextButtonClicked()
{
	if (CurrentIndex < Items.Num() - 1)
	{
		CurrentIndex++;
		SetCurrentItem(CurrentIndex);
	}
}
