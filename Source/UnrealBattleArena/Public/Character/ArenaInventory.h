// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArenaInventory.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALBATTLEARENA_API UArenaInventory : public UActorComponent
{
	GENERATED_BODY()

public:	

	UArenaInventory();

	int32 GetNumWeapons() const;
	bool HasWeapon(TSubclassOf<class AArenaWeapon> WeaponClass) const;
	
	class AArenaWeapon* GetWeapon(int32 WeaponIndex) const;
	class AArenaWeapon* GetWeapon(TSubclassOf<class AArenaWeapon> WeaponClass) const;
	
	void AddWeapon(class AArenaWeapon* Weapon);
	void RemoveAllWeapons();

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(Replicated)
	TArray<class AArenaWeapon*> Weapons;

	UPROPERTY(Replicated)
	class AArenaCharacter* Character{ nullptr };
};
