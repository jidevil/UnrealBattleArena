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
	FORCEINLINE class UArenaUIManager* GetUIManager() const { return UIManager; }

	void ShowMainMenu();
	void ShowLobby();
	void ShowHUD();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerJoinMatch();
	
	UFUNCTION(Client, Reliable)
	void ClientOnMainMenu();

	UFUNCTION(Client, Reliable)
	void ClientOnLobby();

	UFUNCTION(Client, Reliable)
	void ClientOnGameplay();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Player Controller")
	TSubclassOf<class UArenaUIManager> UIManagerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Player Controller")
	TSubclassOf<class UArenaHUD> PlayerHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Player Controller")
	TSubclassOf<class UArenaMainMenu> MainMenuClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Player Controller")
	TSubclassOf<class UArenaLobby> LobbyClass;
	
	UPROPERTY(Transient)
	class UArenaHUD* PlayerHUD{ nullptr };

	UPROPERTY(Transient)
	class UArenaUIManager* UIManager{ nullptr };
};
