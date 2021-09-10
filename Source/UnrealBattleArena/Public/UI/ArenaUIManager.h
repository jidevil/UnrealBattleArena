// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/ArenaDialog.h"
#include "ArenaUIManager.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALBATTLEARENA_API UArenaUIManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UArenaUIManager();

	UFUNCTION(BlueprintCallable)
	UArenaDialog* CreateDialog(EDialogType DialogType, FDialogOnButtonClickDelegate OnButtonClickDelegate,
		TSubclassOf<UArenaDialog> DialogClass = nullptr);

	UFUNCTION(BlueprintCallable)
	class UArenaScreen* PushScreen(TSubclassOf<class UArenaScreen> ScreenClass);

	UFUNCTION(BlueprintCallable)
	class UArenaScreen* PopScreen();

	UFUNCTION(BlueprintCallable)
	void PushToBack(TSubclassOf<class UArenaScreen> ScreenClass);

	UFUNCTION(BlueprintCallable)
	void PopToFront(TSubclassOf<class UArenaScreen> ScreenClass);

	UFUNCTION(BlueprintCallable)
	bool HasScreen(TSubclassOf<class UArenaScreen> ScreenClass);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena UI Manager")
	TSubclassOf<class UArenaDialog> DefaultDialogClass;

	TArray<class UArenaScreen*> Screens;
};
