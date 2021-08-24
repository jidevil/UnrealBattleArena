// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArenaCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth{ 1.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxStamina{ 1.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxShield{ 1.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxSpeedMultiplier{ 4.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StaminaDepletionRate{ 0.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StaminRegenRate{ 0.0f };
};

USTRUCT(BlueprintType)
struct FCharacterAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Death{ nullptr };
};

USTRUCT(BlueprintType)
struct FCharacterHitInfo
{
	GENERATED_BODY()

	UPROPERTY(Transient, BlueprintReadOnly)
	AActor* DamageCauser{ nullptr };

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bKilled{ false };
	
	UPROPERTY(Transient, BlueprintReadOnly)
	float Damage{ 0.0f };
};

UCLASS()
class UNREALBATTLEARENA_API AArenaCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AArenaCharacter();

	FORCEINLINE class UCameraComponent* GetCamera1P() const { return Camera1P; };
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; };
	FORCEINLINE const FRotator& GetLookRotation() const { return LookRotation; }
	FORCEINLINE const FCharacterAttributes& GetAttributes() const { return Attributes; }
	FORCEINLINE bool IsSprinting() const { return bSprinting; }

	class USkeletalMeshComponent* GetCharacterMesh() const;
	class UCameraComponent* GetCamera() const;
	class UArenaInventory* GetInventory() const;
	class UArenaAbilitySystem* GetAbilitySystem() const;
	
	float GetCurrentHealth() const;
	float GetCurrentStamina() const;
	float GetCurrentShield() const;
	float GetCurrentSpeedMultiplier() const;
	
	bool IsAlive() const;
	bool IsEnemyFor(AController* OtherController) const;

	class AArenaWeapon* GetCurrentWeapon() const;
	void AddWeapon(TSubclassOf<class AArenaWeapon> WeaponClass);
	void EquipWeapon(int32 WeaponIndex);
	void EquipNextWeapon();

	void SetAttributes(const FCharacterAttributes& InAttributes);
	void SetSprinting(bool bInSprinting);
	void SetMaxHealth(float InMaxHealth);
	void SetMaxStamina(float InMaxStamina);
	void SetHealth(float InHealth);
	void SetStamina(float InStamina);
	void SetSheild(float InShield);
	void SetSpeedMultiplier(float InSpeedMultiplier);

	void AddHealth(float InHealth);
	void AddStamina(float InStamina);
	void AddShield(float InShield);
	void AddSpeedMultiplier(float InSpeedMultiplier);

	void ToggleView();
	void SetCameraFOV(float FOV);
	void UpdateSpeed();
	void UpdateStamina(bool bDeplete, float DeltaSeconds);
	void Die();
	
	virtual bool IsFirstPersonView() const;
	virtual void GetAimLocationAndRotation(FVector& AimLocation, FVector& AimRotation) const;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.0f,
		FName StartSectionName = NAME_None) override;
	virtual void StopAllAnimMontages();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Reload();
	void BeginSprint();
	void EndSprint();
	void BeginJump();
	void EndJump();
	void BeginFire();
	void EndFire();
	void BeginIronsights();
	void EndIronsights();
	void SpawnDefaultInventory();
	void DestroyInventory();
	void UpdateHealthHUD();
	void UpdateStaminaHUD();
	void UpdateShieldHUD();
	void SetRagdollPhysics();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEquipWeapon(int32 NextWeaponIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSprinting(bool bInSprinting);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateSpeed();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateStamina(bool bDeplete);

	UFUNCTION(Client, Reliable)
	void ClientSetSprinting(bool bInSprinting);

	UFUNCTION(Client, Reliable)
	void ClientToggleView();

	UFUNCTION(NetMulticast, Unreliable, WithValidation)
	void MulticastLookAtRotation(FRotator InLookRotation);

	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_CurrentStamina();

	UFUNCTION()
	void OnRep_CurrentShield();

	UFUNCTION()
	void OnRep_CurrentSpeedMultiplier();

	UFUNCTION()
	void OnRep_HitInfo();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Character")
	class USkeletalMeshComponent* Mesh1P{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Character")
	class UCameraComponent* Camera1P{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Character")
	class UCameraComponent* Camera3P{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Character")
	class USpringArmComponent* SpringArm{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Character")
	class UArenaInventory* Inventory{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Character")
	class UArenaAbilitySystem* AbilitySystem{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Character")
	FCharacterAnimations Animations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Character")
	FCharacterAttributes Attributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arena Character")
	TArray<TSubclassOf<class AArenaWeapon>> DefaultWeapons;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth{ 0.0f };

	UPROPERTY(ReplicatedUsing=OnRep_CurrentStamina)
	float CurrentStamina{ 0.0f };

	UPROPERTY(ReplicatedUsing=OnRep_CurrentShield)
	float CurrentShield{ 0.0f };

	UPROPERTY(ReplicatedUsing=OnRep_CurrentSpeedMultiplier)
	float CurrentSpeedMultiplier{ 1.0f };

	UPROPERTY(ReplicatedUsing=OnRep_HitInfo)
	FCharacterHitInfo HitInfo;

	UPROPERTY(Replicated)
	int32 CurrentWeaponIndex{ -1 };

	UPROPERTY(Replicated)
	bool bSprinting{ false };

	UPROPERTY(Transient)
	class UArenaPlayerWidget* PlayerWidget{ nullptr };

	FRotator LookRotation;
	bool bChangeFOV{ false };
	float TargetFOV{ 0.0f };
	bool bFirstPersonView{ false };
	bool bDepleteStamina{ false };
	bool bRegenStamina{ false };
	bool bIsDying{ false };
};
