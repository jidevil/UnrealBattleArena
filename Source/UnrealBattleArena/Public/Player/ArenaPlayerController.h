// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArenaPlayerController.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	FORCEINLINE class UArenaHUD* GetPlayerHUD() const { return PlayerHUD; }
	virtual void PawnPendingDestroy(APawn* InPawn) override;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void UnFreeze() override;

	UFUNCTION(Client, Reliable)
	void ClientSetSpectatorCamera(FVector CameraLocation, FRotator CameraRotation);
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Player Controller")
	TSubclassOf<class UArenaHUD> PlayerHUDClass;

	UPROPERTY(Transient)
	class UArenaHUD* PlayerHUD{ nullptr };
};
