// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Idle UMETA(DisplayName="Idle"),
	Equip UMETA(DisplayName="Equip"),
	UnEquip UMETA(DisplayName="UnEquip"),
	Reload UMETA(DisplayName="Reload"),
	Fire UMETA(DisplayName="Fire")
};

USTRUCT(BlueprintType)
struct FWeaponCharacterAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Equip{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Reload{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Fire{ nullptr };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* FireIronsights{ nullptr };
};

USTRUCT(BlueprintType)
struct FWeaponAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimSequence* Reload{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimSequence* Fire{ nullptr };
};

USTRUCT(BlueprintType)
struct FWeaponImpactEffect
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* Particle{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* Sound{ nullptr };
};

USTRUCT(BlueprintType)
struct FWeaponAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bProjectileDamage{ false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BulletRange{ 10000.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BulletSpread{ 0.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float IronsightsBulletSpread{ 0.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float IronsightsFOV{ 60.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ShotDuration{ 0.5f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AmmoPerClip{ 0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmoCount{ 0 };
};

USTRUCT(BlueprintType)
struct FWeaponDamageAttributes
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bRadialDamage{ false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DefaultDamage{ 0.1f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageRadius{ 0.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HeadshotMultiplier{ 1.25f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BodyshotMultiplier{ 1.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FootshotMultiplier{ 0.75f };
};

USTRUCT(BlueprintType)
struct FWeaponHitInfo
{
	GENERATED_BODY()

	UPROPERTY(Transient, BlueprintReadOnly)
	FVector AimLocation;

	UPROPERTY(Transient, BlueprintReadOnly)
	FVector AimRotation;	
};

UCLASS()
class UNREALBATTLEARENA_API AArenaWeapon : public AActor
{
	GENERATED_BODY()
	
public:

	friend class AArenaWeaponProjectile;

	AArenaWeapon();

	class USkeletalMeshComponent* GetMesh() const;
	const FName& GetInventorySocket() const;
	const FWeaponAttributes& GetAttributes() const;
	class AArenaCharacter* GetCharacter() const;

	bool HasLeftHandIK() const;
	bool IsInIronsights() const;
	bool IsReady() const;

	bool CanEquip() const;
	bool CanUnEquip() const;
	bool CanReload() const;
	bool CanFire() const;
	bool CanIronsights() const;
	bool CanAddClip() const;

	int32 GetRemainingAmmo() const;
	void AttachToPlayer();
	void AddClips(int32 ClipCount);
	void ToggleView();
	
	virtual void Equip();
	virtual void UnEquip();
	virtual void Reload();
	virtual void BeginFire();
	virtual void EndFire();
	virtual void BeginIronsights();
	virtual void EndIronsights();

protected:

	virtual void BeginPlay() override;
	virtual void StopReload();
	virtual void StopFire();
	
	virtual void Fire(const FVector& AimLocation, const FVector& AimRotation);
	virtual void ApplyDamage(const FHitResult& HitResult);
	virtual void SpawnImpactEffects(const FHitResult& HitResult);
	virtual FHitResult WeaponTrace(const FVector& TraceStart, const FVector& TraceEnd);
	virtual void UpdateAmmoHUD();

	virtual void OnEquip();
	virtual void OnEquipFinished();
	virtual void OnUnEquip();
	virtual void OnUnEquipFinished();
	virtual void OnReload();
	virtual void OnReloadFinished();
	virtual void OnFire();
	virtual void OnFireFinished();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleView();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEquip();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUnEquip();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginIronsights();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndIronsights();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopReload();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopFire();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAddClips(int32 ClipCount);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnFire(const FVector& AimLocation, const FVector& AimRotation);

	UFUNCTION(Client, Reliable)
	void ClientReload();

	UFUNCTION(Client, Reliable)
	void ClientStopFire();

	UFUNCTION()
	void OnRep_ToggleView();

	UFUNCTION()
	void OnRep_Ironsights();

	UFUNCTION()
	void OnRep_Equipping();

	UFUNCTION()
	void OnRep_UnEquipping();

	UFUNCTION()
	void OnRep_Reloading();

	UFUNCTION()
	void OnRep_FireCount();

	UFUNCTION()
	void OnRep_CurrentAmmoInClip();

	UFUNCTION()
	void OnRep_RemainingAmmo();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	class USkeletalMeshComponent* Mesh1P{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	class USkeletalMeshComponent* Mesh3P{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	TSubclassOf<class AArenaWeaponProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	FName WeaponSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	FName WeaponIronsightSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	FName WeaponInventorySocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	FName MuzzleSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	FWeaponAttributes Attributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	FWeaponDamageAttributes DamageAttributes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	FWeaponCharacterAnimations CharacterAnimations1P;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	FWeaponCharacterAnimations CharacterAnimations3P;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	FWeaponAnimations Animations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Weapon")
	TMap<TEnumAsByte<EPhysicalSurface>, FWeaponImpactEffect> ImpactEffects;

	UPROPERTY(Replicated)
	class AArenaCharacter* ArenaCharacter{ nullptr };
	
	UPROPERTY(Replicated)
	bool bEquipped{ false };	
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentAmmoInClip)
	int32 CurrentAmmoInClip{ 0 };

	UPROPERTY(ReplicatedUsing=OnRep_RemainingAmmo)
	int32 RemainingAmmo{ 0 };

	UPROPERTY(ReplicatedUsing=OnRep_ToggleView)
	bool bToggleView{ false };

	UPROPERTY(ReplicatedUsing=OnRep_Ironsights)
	bool bIronsights{ false };

	UPROPERTY(ReplicatedUsing=OnRep_Equipping)
	bool bEquipping{ false };

	UPROPERTY(ReplicatedUsing=OnRep_UnEquipping)
	bool bUnEquipping{ false };

	UPROPERTY(ReplicatedUsing=OnRep_Reloading)
	bool bReloading{ false };

	UPROPERTY(ReplicatedUsing=OnRep_FireCount)
	int32 FireCount{ 0 };

	UPROPERTY(Replicated)
	FWeaponHitInfo HitInfo;

	UPROPERTY(Transient)
	class UArenaWeaponWidget* WeaponWidget{ nullptr };

	FTimerHandle EquipTimer;
	FTimerHandle UnEquipTimer;
	FTimerHandle EquipFinishedTimer;
	FTimerHandle UnEquipFinishedTimer;
	FTimerHandle ReloadTimer;
	FTimerHandle ReloadFinishedTimer;
	FTimerHandle FireFinishedTimer;
	bool bHasLeftHandIK{ false };
};
