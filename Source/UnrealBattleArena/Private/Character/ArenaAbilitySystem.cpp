// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ArenaAbilitySystem.h"
#include "Character/ArenaAbility.h"
#include "Net/UnrealNetwork.h"

UArenaAbilitySystem::UArenaAbilitySystem()
{
	SetIsReplicatedByDefault(true);
}

bool UArenaAbilitySystem::IsAbilityRunning(TSubclassOf<UArenaAbility> AbilityClass)
{
	for (const UArenaAbility* Ability : RunningAbilities)
	{
		if (Ability->GetClass() == AbilityClass)
		{
			return true;
		}
	}

	return false;
}

bool UArenaAbilitySystem::AddAbility(TSubclassOf<UArenaAbility> AbilityClass, bool bStart)
{
	UArenaAbility* Ability = NewObject<UArenaAbility>(GetOwner(), AbilityClass);
	if (Ability)
	{
		Ability->RegisterComponent();
		Ability->OnAbilityStarted.AddDynamic(this, &UArenaAbilitySystem::OnAbilityStarted);
		Ability->OnAbilityEnded.AddDynamic(this, &UArenaAbilitySystem::OnAbilityEnded);
		
		if (bStart && Ability->CanStart())
		{
			Ability->Start();
			return true;
		}
	}

	return false;
}

void UArenaAbilitySystem::OnAbilityStarted(UArenaAbility* Ability)
{
	if (Ability)
	{
		RunningAbilities.Add(Ability);
	}
}

void UArenaAbilitySystem::OnAbilityEnded(UArenaAbility* Ability)
{
	if (Ability)
	{
		Ability->UnregisterComponent();
		Ability->DestroyComponent();
		RunningAbilities.Remove(Ability);
	}
}

void UArenaAbilitySystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UArenaAbilitySystem, RunningAbilities);
}