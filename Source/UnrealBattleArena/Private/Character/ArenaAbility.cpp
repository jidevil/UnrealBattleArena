// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ArenaAbility.h"
#include "Character/ArenaCharacter.h"
#include "Net/UnrealNetwork.h"

UArenaAbility::UArenaAbility()
{
	SetIsReplicatedByDefault(true);
}

bool UArenaAbility::CanStart() const
{
	return true;
}

bool UArenaAbility::Start()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerStart();
	}
	else
	{
		OnStarted();
	
		if (AbilityType == EAbilityType::Instant)
		{
			OnStopped();
		}
		else
		{
			FTimerDelegate DurationCallback;
			DurationCallback.BindLambda([this]
			{
				OnStopped();
			});

			FTimerHandle DurationTimer;
			GetWorld()->GetTimerManager().SetTimer(DurationTimer, DurationCallback,
				Duration, false);
		}
	}

	return true;
}

void UArenaAbility::BeginPlay()
{
	Super::BeginPlay();

	ArenaCharacter = Cast<AArenaCharacter>(GetOwner());
	if (ArenaCharacter && ArenaCharacter->IsLocallyControlled())
	{
		SetupInput();
	}
}

void UArenaAbility::RunAbility()
{
	if (CanStart())
	{
		Start();
	}
}

void UArenaAbility::OnStarted()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		bStarted = true;
		if (OnAbilityStarted.IsBound())
		{
			OnAbilityStarted.Broadcast(this);	
		}
	}
	
	StartEffects();
}

void UArenaAbility::OnStopped()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		bStarted = false;
		if (OnAbilityEnded.IsBound())
		{
			OnAbilityEnded.Broadcast(this);
		}
	}
	
	StopEffects();
}

void UArenaAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UArenaAbility, ArenaCharacter);
	DOREPLIFETIME(UArenaAbility, bStarted);
}

void UArenaAbility::SetupInput()
{
	UInputComponent* InputComponent = ArenaCharacter->InputComponent;
	if (InputComponent && !InputAction.IsNone())
	{
		InputComponent->BindAction(InputAction, IE_Pressed, this, &UArenaAbility::RunAbility);
	}
}

void UArenaAbility::StartEffects()
{
}

void UArenaAbility::StopEffects()
{
}

void UArenaAbility::ServerStart_Implementation()
{
	Start();
}

bool UArenaAbility::ServerStart_Validate()
{
	return true;
}

void UArenaAbility::OnRep_Started()
{
	if (bStarted)
	{
		OnStarted();
	}
	else
	{
		OnStopped();
	}
}