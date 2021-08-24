// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaWeaponProjectile.generated.h"

UCLASS()
class UNREALBATTLEARENA_API AArenaWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AArenaWeaponProjectile();

protected:

	virtual void BeginPlay() override;

protected:

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon Projectile")
	class USphereComponent* SphereBody{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon Projectile")
	class UParticleSystemComponent* TrailEffect{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon Projectile")
	class UProjectileMovementComponent* ProjectileMovement{ nullptr };

	UPROPERTY(Replicated, Transient, BlueprintReadOnly, Category="Arena Weapon Projectile")
	class AArenaWeapon* Weapon{ nullptr };
};
