// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSystem/ArenaWaveSystem.h"
#include "WaveSystem/ArenaWave.h"
#include "Bot/ArenaBotCharacter.h"
#include "Bot/ArenaBotController.h"
#include "GameModes/ArenaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Online/ArenaPlayerStart.h"

void AArenaWaveSystem::StartWaves()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		for (UArenaWave* Wave : Waves)
		{
			if (Wave->WaveDescription->bAutoTrigger)
			{
				Wave->Start();
			}
		}
	}
}

void AArenaWaveSystem::SpawnBot(const FName& BotName, int32 SpawnPointIndex)
{
	AArenaGameMode* GameMode = Cast<AArenaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
	{
		return;
	}

	FBotDescription* BotDescription = BotDescriptions->FindRow<FBotDescription>(BotName,
		"ArenaWaveSystem");

	if (BotDescription)
	{
		AArenaBotController* BotController = GetWorld()->SpawnActor<AArenaBotController>(
			BotDescription->BotControllerClass);

		if (BotController)
		{
			AArenaPlayerState* PlayerState = Cast<AArenaPlayerState>(BotController->PlayerState);
			if (PlayerState)
			{
				PlayerState->SetIsBot(true);
				PlayerState->SetTeamNumber(GameMode->ChooseTeam(PlayerState));
			}
			
			const AArenaPlayerStart* SpawnPoint = SpawnPoints[SpawnPointIndex];

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SpawnPoint->GetActorLocation());
			SpawnTransform.SetRotation(SpawnPoint->GetActorRotation().Quaternion());

			AArenaBotCharacter* BotCharacter = GetWorld()->SpawnActorDeferred<AArenaBotCharacter>(
				BotDescription->BotCharacterClass, SpawnTransform, this, nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			if (BotCharacter)
			{
				BotCharacter->SetAttributes(BotDescription->Attributes);
				BotCharacter->FinishSpawning(SpawnTransform);
				BotCharacter->EquipNextWeapon();
				BotController->Possess(BotCharacter);
			}
		}
	}
}

void AArenaWaveSystem::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		check(SpawnPoints.Num() > 0);
		check(BotDescriptions);
		check(WaveDescriptions);

		for (const FName& WaveName : WaveDescriptions->GetRowNames())
		{
			FWaveDescription* WaveDescription = WaveDescriptions->FindRow<FWaveDescription>(WaveName,
				"ArenaWaveSystem");

			if (WaveDescription)
			{
				UArenaWave* Wave = NewObject<UArenaWave>(this);
				Waves.Add(Wave);

				Wave->RegisterComponent();
				Wave->WaveDescription = WaveDescription;
			}
		}		
	}
}
