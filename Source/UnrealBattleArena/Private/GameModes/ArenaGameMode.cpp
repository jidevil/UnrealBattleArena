// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ArenaGameMode.h"
#include "Character/ArenaCharacter.h"
#include "Player/ArenaPlayerController.h"
#include "Player/ArenaPlayerState.h"
#include "Bot/ArenaBotController.h"
#include "Components/CapsuleComponent.h"
#include "Online/ArenaPlayerStart.h"
#include "EngineUtils.h"
#include "Online/ArenaSpectatorPawn.h"
#include "Player/ArenaPlayerCharacter.h"

AArenaGameMode::AArenaGameMode()
{
	DefaultPawnClass = AArenaCharacter::StaticClass();
	PlayerControllerClass = AArenaPlayerController::StaticClass();
	PlayerStateClass = AArenaPlayerState::StaticClass();
	SpectatorClass = AArenaSpectatorPawn::StaticClass();
}

void AArenaGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AArenaPlayerState* PlayerState = Cast<AArenaPlayerState>(NewPlayer);
	if (PlayerState)
	{
		PlayerState->SetIsBot(false);
		PlayerState->SetTeamNumber(ChooseTeam(PlayerState));
	}
}

void AArenaGameMode::InitGameState()
{
	Super::InitGameState();
}

void AArenaGameMode::PlayerDied(AController* Controller)
{
	if (!Controller)
	{
		return;
	}
	
	const FTransform& SpawnTransform = Controller->GetPawn()->GetActorTransform();
	AArenaSpectatorPawn* SpectatorPawn = GetWorld()->SpawnActorDeferred<AArenaSpectatorPawn>(SpectatorClass,
		SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (SpectatorPawn)
	{
		Controller->UnPossess();
		SpectatorPawn->FinishSpawning(SpawnTransform);
		Controller->Possess(SpectatorPawn);
	}

	FTimerDelegate RespawnCallback;
	RespawnCallback.BindUObject(this, &AArenaGameMode::RespawnPlayer, Controller);

	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, RespawnCallback, GetRespawnTime(Controller), false);
}

void AArenaGameMode::RespawnPlayer(AController* Controller)
{
	if (Controller->IsPlayerController())
	{
		const AActor* PlayerStart = FindPlayerStart(Controller);

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(PlayerStart->GetActorLocation());
		SpawnTransform.SetRotation(PlayerStart->GetActorRotation().Quaternion());

		AArenaPlayerCharacter* PlayerCharacter = GetWorld()->SpawnActorDeferred<AArenaPlayerCharacter>(DefaultPawnClass,
			SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (PlayerCharacter)
		{
			PlayerCharacter->FinishSpawning(SpawnTransform);
			
			APawn* SpectatorPawn = Controller->GetPawn();
			Controller->UnPossess();
			SpectatorPawn->Destroy();
			Controller->Possess(PlayerCharacter);
		}
	}
}

bool AArenaGameMode::CanDealDamage(AArenaPlayerState* DamageInstigator, AArenaPlayerState* DamagedPlayer) const
{
	if (DamageInstigator && DamagedPlayer)
	{
		return DamageInstigator->GetTeamNumber() != DamagedPlayer->GetTeamNumber();
	}

	return false;
}

int32 AArenaGameMode::ChooseTeam(AArenaPlayerState* PlayerState)
{
	return -1;
}

AActor* AArenaGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PreferredSpawnPoints;
	TArray<APlayerStart*> FallbackSpawnPoints;

	for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
	{
		if (IsSpawnPointAllowed(PlayerStart, Player))
		{
			if (IsSpawnPointPreferred(PlayerStart, Player))
			{
				PreferredSpawnPoints.Add(PlayerStart);
			}
			else
			{
				FallbackSpawnPoints.Add(PlayerStart);
			}
		}
	}

	if (PreferredSpawnPoints.Num() > 0)
	{
		return PreferredSpawnPoints[FMath::RandHelper(PreferredSpawnPoints.Num())];
	}

	if (FallbackSpawnPoints.Num() > 0)
	{
		return FallbackSpawnPoints[FMath::RandHelper(FallbackSpawnPoints.Num())];
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

bool AArenaGameMode::IsSpawnPointAllowed(APlayerStart* PlayerStart, AController* Controller) const
{
	AArenaPlayerStart* ArenaPlayerStart = Cast<AArenaPlayerStart>(PlayerStart);
	if (ArenaPlayerStart)
	{
		AArenaPlayerState* PlayerState = Cast<AArenaPlayerState>(Controller->PlayerState);
		if (PlayerState && ArenaPlayerStart->GetTeamNumber() == PlayerState->GetTeamNumber())
		{
			return true;
		}
	}

	return false;
}

bool AArenaGameMode::IsSpawnPointPreferred(APlayerStart* PlayerStart, AController* Controller) const
{
	AArenaCharacter* DefaultCharacter = AArenaCharacter::StaticClass()->GetDefaultObject<AArenaCharacter>();
	if (DefaultCharacter)
	{
		const FVector SpawnLocation = PlayerStart->GetActorLocation();
		for (AArenaCharacter* OtherCharacter : TActorRange<AArenaCharacter>(GetWorld()))
		{
			if (OtherCharacter != DefaultCharacter)
			{
				const float CombinedHeight = (DefaultCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() +
					OtherCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * 2.0f;

				const float CombinedRadius = DefaultCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() +
					OtherCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

				const FVector OtherLocation = OtherCharacter->GetActorLocation();

				if (FMath::Abs(SpawnLocation.Z - OtherLocation.Z) < CombinedHeight &&
					(SpawnLocation - OtherLocation).Size2D() < CombinedRadius)
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

float AArenaGameMode::GetRespawnTime(AController* Controller)
{
	return DefaultRespawnTime;
}

