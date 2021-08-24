// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ArenaCharacter.h"
#include "Character/ArenaInventory.h"
#include "Character/ArenaAbilitySystem.h"
#include "Weapons/ArenaWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "UnrealBattleArena.h"
#include "GameModes/ArenaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ArenaPlayerController.h"
#include "Player/ArenaPlayerState.h"
#include "UI/HUD/ArenaHUD.h"
#include "UI/HUD/Widgets/ArenaPlayerWidget.h"

AArenaCharacter::AArenaCharacter()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->JumpZVelocity = CHARACTER_DEFAULT_JUMP_VELOCITY;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = CHARACTER_DEFAULT_WALK_SPEED;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CHARACTER_DEFAULT_CROUCH_SPEED;
	GetCharacterMovement()->NavAgentProps.bCanJump = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	Camera1P = CreateDefaultSubobject<UCameraComponent>("Camera1P");
	Camera1P->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	Camera1P->SetupAttachment(GetCapsuleComponent());
	Camera1P->bAutoActivate = false;
	Camera1P->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh1P");
	Mesh1P->SetupAttachment(Camera1P);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bAutoActivate = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset.Set(0.0f, 80.0f, 00.0f);

	Camera3P = CreateDefaultSubobject<UCameraComponent>("Camera3P");
	Camera3P->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera3P->bAutoActivate = false;
	Camera3P->bUsePawnControlRotation = false;
	
	Inventory = CreateDefaultSubobject<UArenaInventory>("Inventory");
	AbilitySystem = CreateDefaultSubobject<UArenaAbilitySystem>("AbilitySystem");
}

bool AArenaCharacter::IsFirstPersonView() const
{
	return IsLocallyControlled() && bFirstPersonView;
}

USkeletalMeshComponent* AArenaCharacter::GetCharacterMesh() const
{
	return IsFirstPersonView() ? Mesh1P : GetMesh();
}

UCameraComponent* AArenaCharacter::GetCamera() const
{
	return IsFirstPersonView() ? Camera1P : Camera3P;
}

UArenaInventory* AArenaCharacter::GetInventory() const
{
	return Inventory;
}

UArenaAbilitySystem* AArenaCharacter::GetAbilitySystem() const
{
	return AbilitySystem;
}

float AArenaCharacter::GetCurrentHealth() const
{
	return CurrentHealth;
}

float AArenaCharacter::GetCurrentStamina() const
{
	return CurrentStamina;
}

float AArenaCharacter::GetCurrentShield() const
{
	return CurrentShield;
}

float AArenaCharacter::GetCurrentSpeedMultiplier() const
{
	return CurrentSpeedMultiplier;
}

bool AArenaCharacter::IsAlive() const
{
	return CurrentHealth > 0.0f;
}

bool AArenaCharacter::IsEnemyFor(AController* OtherController) const
{
	bool bEnemy = false;
	
	if (Controller == OtherController || !OtherController)
	{
		return bEnemy;
	}

	AArenaPlayerState* ThisPlayerState = Cast<AArenaPlayerState>(Controller->PlayerState);
	AArenaPlayerState* OtherPlayerState = Cast<AArenaPlayerState>(OtherController->PlayerState);

	AArenaGameMode* GameMode = Cast<AArenaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		bEnemy = GameMode->CanDealDamage(OtherPlayerState, ThisPlayerState);
	}

	return bEnemy;
}

float AArenaCharacter::PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	USkeletalMeshComponent* PlayerMesh = GetCharacterMesh();
	if (AnimMontage && PlayerMesh && PlayerMesh->GetAnimInstance())
	{
		return PlayerMesh->GetAnimInstance()->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void AArenaCharacter::StopAllAnimMontages()
{
	USkeletalMeshComponent* CharacterMesh = GetCharacterMesh();
	if (CharacterMesh && CharacterMesh->GetAnimInstance())
	{
		CharacterMesh->GetAnimInstance()->StopAllMontages(0.0f);
	}
}

AArenaWeapon* AArenaCharacter::GetCurrentWeapon() const
{
	return Inventory->GetWeapon(CurrentWeaponIndex);
}

void AArenaCharacter::AddWeapon(TSubclassOf<AArenaWeapon> WeaponClass)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}
	
	if (!Inventory->HasWeapon(WeaponClass))
	{
		AArenaWeapon* Weapon = GetWorld()->SpawnActorDeferred<AArenaWeapon>(WeaponClass, FTransform::Identity,
			this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (Weapon)
		{
			Weapon->FinishSpawning(FTransform::Identity);
			Weapon->AttachToPlayer();
			Inventory->AddWeapon(Weapon);
		}
	}
}

void AArenaCharacter::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex == CurrentWeaponIndex)
	{
		return;
	}
	
	AArenaWeapon* CurrentWeapon = Inventory->GetWeapon(CurrentWeaponIndex);
	AArenaWeapon* NextWeapon = Inventory->GetWeapon(WeaponIndex);
	
	if (NextWeapon && NextWeapon->CanEquip())
	{
		if (CurrentWeapon)
		{
			if (!CurrentWeapon->CanUnEquip())
			{
				return;
			}

			CurrentWeapon->UnEquip();
		}
		
		if (GetLocalRole() < ROLE_Authority)
		{
			ServerEquipWeapon(WeaponIndex);
		}
		else
		{
			CurrentWeaponIndex = WeaponIndex;
		}

		NextWeapon->Equip();
	}
}

void AArenaCharacter::EquipNextWeapon()
{
	EquipWeapon((CurrentWeaponIndex + 1) % Inventory->GetNumWeapons());
}

void AArenaCharacter::SetAttributes(const FCharacterAttributes& InAttributes)
{
	Attributes = InAttributes;
}

void AArenaCharacter::SetSprinting(bool bInSprinting)
{
	if (GetLocalRole() < ROLE_Authority && IsLocallyControlled())
	{
		ServerSetSprinting(bInSprinting);
	}

	bSprinting = bInSprinting;
	UpdateSpeed();

	if (bSprinting)
	{
		ServerUpdateStamina(true);
	}
	else
	{
		ServerUpdateStamina(false);
	}
}

void AArenaCharacter::SetMaxHealth(float MaxHealth)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Attributes.MaxHealth = MaxHealth;
	}
}

void AArenaCharacter::SetMaxStamina(float InMaxStamina)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Attributes.MaxStamina = InMaxStamina;
	}
}

void AArenaCharacter::SetHealth(float InHealth)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(InHealth, 0.0f, Attributes.MaxHealth);
	}
}

void AArenaCharacter::SetStamina(float InStamina)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentStamina = FMath::Clamp(InStamina, 0.0f, Attributes.MaxStamina);
	}
}

void AArenaCharacter::SetSheild(float InShield)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentShield = FMath::Clamp(InShield, 0.0f, Attributes.MaxShield);
	}
}

void AArenaCharacter::SetSpeedMultiplier(float InSpeedMultiplier)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentSpeedMultiplier = FMath::Clamp(InSpeedMultiplier, 1.0f, Attributes.MaxSpeedMultiplier);
		UpdateSpeed();
	}
}

void AArenaCharacter::AddHealth(float InHealth)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth += InHealth;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, Attributes.MaxHealth);
	}
}

void AArenaCharacter::AddStamina(float InStamina)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentStamina += InStamina;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, Attributes.MaxStamina);
	}
}

void AArenaCharacter::AddShield(float InShield)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentShield += InShield;
		CurrentShield = FMath::Clamp(CurrentShield, 0.0f, Attributes.MaxShield);
	}
}

void AArenaCharacter::AddSpeedMultiplier(float InSpeedMultiplier)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentSpeedMultiplier *= InSpeedMultiplier;
		CurrentSpeedMultiplier = FMath::Clamp(CurrentSpeedMultiplier, 1.0f,
			Attributes.MaxSpeedMultiplier);

		UpdateSpeed();
	}
}

void AArenaCharacter::ToggleView()
{
	AArenaWeapon* CurrentWeapon = Inventory->GetWeapon(CurrentWeaponIndex);
	if (CurrentWeapon && !CurrentWeapon->IsReady())
	{
		return;
	}
	
	bFirstPersonView = !bFirstPersonView;
	if (bFirstPersonView)
	{
		Camera3P->Deactivate();
		SpringArm->Deactivate();

		GetMesh()->SetOwnerNoSee(true);
		GetMesh()->SetOnlyOwnerSee(false);
		GetMesh()->CastShadow = true;

		GetMesh1P()->SetHiddenInGame(false);
		GetMesh1P()->SetOwnerNoSee(false);
		GetMesh1P()->SetOnlyOwnerSee(true);
		GetMesh1P()->CastShadow = false;

		Camera1P->Activate();
	}
	else
	{
		Camera1P->Deactivate();

		GetMesh()->SetOwnerNoSee(false);
		GetMesh()->SetOnlyOwnerSee(false);
		GetMesh()->CastShadow = true;

		GetMesh1P()->SetHiddenInGame(true);
		GetMesh1P()->CastShadow = false;

		SpringArm->Activate();
		Camera3P->Activate();
	}

	for (int32 Index = 0; Index < Inventory->GetNumWeapons(); Index++)
	{
		AArenaWeapon* Weapon = Inventory->GetWeapon(Index);
		if (Weapon)
		{
			Weapon->ToggleView();
		}
	}
}

void AArenaCharacter::SetCameraFOV(float FOV)
{
	TargetFOV = FOV;
	bChangeFOV = true;
}

void AArenaCharacter::BeginPlay()
{
	Super::BeginPlay();
	ToggleView();
	
	if (GetLocalRole() < ROLE_Authority && IsLocallyControlled())
	{
		AArenaPlayerController* PlayerController = Cast<AArenaPlayerController>(GetController());
		if (PlayerController)
		{
			UArenaHUD* PlayerHUD = PlayerController->GetPlayerHUD();
			if (PlayerHUD)
			{
				PlayerWidget = PlayerHUD->GetPlayerWidget();
			}
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = Attributes.MaxHealth;
		CurrentStamina = Attributes.MaxStamina;
		CurrentShield = Attributes.MaxShield;
	}

	SpawnDefaultInventory();
	UpdateHealthHUD();
	UpdateStaminaHUD();
	UpdateShieldHUD();
}

void AArenaCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ROLE_Authority)
	{
		MulticastLookAtRotation(GetControlRotation());

		if (bDepleteStamina)
		{
			UpdateStamina(true, DeltaSeconds);
		}

		if (bRegenStamina)
		{
			UpdateStamina(false, DeltaSeconds);
		}
	}
}

void AArenaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AArenaCharacter, CurrentWeaponIndex);
	DOREPLIFETIME(AArenaCharacter, CurrentHealth);
	DOREPLIFETIME(AArenaCharacter, CurrentStamina);
	DOREPLIFETIME(AArenaCharacter, CurrentShield);
	DOREPLIFETIME(AArenaCharacter, CurrentSpeedMultiplier);
	DOREPLIFETIME(AArenaCharacter, bSprinting);
	DOREPLIFETIME(AArenaCharacter, HitInfo);
}

float AArenaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		float RemainingDamage = DamageAmount;
		if (CurrentShield > 0.0f)
		{
			RemainingDamage = DamageAmount - CurrentShield;
			CurrentShield = CurrentShield - DamageAmount;

			RemainingDamage = (RemainingDamage < 0.0f) ? 0.0f : RemainingDamage;
			CurrentShield = FMath::Clamp(CurrentShield, 0.0f, Attributes.MaxShield);
		}

		if (RemainingDamage > 0.0f)
		{
			CurrentHealth -= RemainingDamage;
			CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, Attributes.MaxHealth);

			if (CurrentHealth <= 0.0f)
			{
				HitInfo.bKilled = true;
				HitInfo.Damage = DamageAmount;
				HitInfo.DamageCauser = DamageCauser;

				Die();
			}
		}
	}

	return DamageAmount;
}

void AArenaCharacter::MoveForward(float Value)
{
	const FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AArenaCharacter::MoveRight(float Value)
{
	const FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AArenaCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AArenaCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AArenaCharacter::Reload()
{
	AArenaWeapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon && CurrentWeapon->CanReload())
	{
		CurrentWeapon->Reload();
	}
}

void AArenaCharacter::BeginSprint()
{
	SetSprinting(true);
}

void AArenaCharacter::EndSprint()
{
	SetSprinting(false);
}

void AArenaCharacter::BeginJump()
{
	Jump();
}

void AArenaCharacter::EndJump()
{
	StopJumping();
}

void AArenaCharacter::BeginFire()
{
	AArenaWeapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon && CurrentWeapon->CanFire())
	{
		CurrentWeapon->BeginFire();
	}
}

void AArenaCharacter::EndFire()
{
	AArenaWeapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		CurrentWeapon->EndFire();
	}
}

void AArenaCharacter::BeginIronsights()
{
	AArenaWeapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon && CurrentWeapon->CanIronsights())
	{
		CurrentWeapon->BeginIronsights();
		UpdateSpeed();
	}
}

void AArenaCharacter::EndIronsights()
{
	AArenaWeapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		CurrentWeapon->EndIronsights();
		UpdateSpeed();
	}
}

void AArenaCharacter::SpawnDefaultInventory()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		for (const TSubclassOf<AArenaWeapon>& WeaponClass : DefaultWeapons)
		{
			AddWeapon(WeaponClass);
		}
	}
}

void AArenaCharacter::DestroyInventory()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Inventory->RemoveAllWeapons();
		CurrentWeaponIndex = -1;
	}
}

void AArenaCharacter::UpdateHealthHUD()
{	
	if (PlayerWidget)
	{
		PlayerWidget->SetHealth(CurrentHealth, Attributes.MaxHealth);
	}
}

void AArenaCharacter::UpdateStaminaHUD()
{	
	if (PlayerWidget)
	{
		PlayerWidget->SetStamina(CurrentStamina);
	}
}

void AArenaCharacter::UpdateShieldHUD()
{
	if (PlayerWidget)
	{
		PlayerWidget->SetShield(CurrentShield, Attributes.MaxShield);
	}
}

void AArenaCharacter::SetRagdollPhysics()
{
	bool bInRagdoll;

	if (IsPendingKill() || !GetMesh()->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;

		bInRagdoll = true;
	}

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	if (!bInRagdoll)
	{
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		SetLifeSpan(10.0f);
	}
}

void AArenaCharacter::UpdateSpeed()
{
	if (GetLocalRole() < ROLE_Authority && IsLocallyControlled())
	{
		ServerUpdateSpeed();
	}
	
	AArenaWeapon* CurrentWeapon = Inventory->GetWeapon(CurrentWeaponIndex);		
	if (CurrentWeapon && CurrentWeapon->IsInIronsights())
	{
		GetCharacterMovement()->MaxWalkSpeed = CHARACTER_DEFAULT_IRONSIGHTS_SPEED *
			CurrentSpeedMultiplier;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = (bSprinting ? CHARACTER_DEFAULT_SPRINT_SPEED :
			CHARACTER_DEFAULT_WALK_SPEED) * CurrentSpeedMultiplier;
	}
}

void AArenaCharacter::UpdateStamina(bool bDeplete, float DeltaSeconds)
{
	if (bDeplete)
	{
		CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, 0.0f, DeltaSeconds,
			Attributes.StaminaDepletionRate);

		if (CurrentStamina <= 0.0f)
		{
			CurrentStamina = 0.0f;
			bSprinting = false;
			bDepleteStamina = false;
			bRegenStamina = true;

			ClientSetSprinting(false);
		}
	}
	else
	{
		CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, Attributes.MaxStamina, DeltaSeconds,
			Attributes.StaminRegenRate);

		if (CurrentStamina >= Attributes.MaxStamina)
		{
			CurrentStamina = Attributes.MaxStamina;
			bRegenStamina = false;
		}
	}
}

void AArenaCharacter::Die()
{
	if (bIsDying)
	{
		return;
	}

	SetReplicatingMovement(false);
	TearOff();
	bIsDying = true;

	if (bFirstPersonView && (GetLocalRole() < ROLE_Authority && IsLocallyControlled()))
	{
		ToggleView();
	}

	DestroyInventory();
	StopAllAnimMontages();

	if (GetLocalRole() == ROLE_Authority)
	{
		AArenaGameMode* GameMode = Cast<AArenaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			GameMode->PlayerDied(GetController());		
		}
	}

	GetMesh()->SetCollisionProfileName("Ragdoll");
	SetActorEnableCollision(true);

	const float AnimTime = PlayAnimMontage(Animations.Death);
	if (AnimTime > 0.0f)
	{
		const float TriggerRagdollTime = AnimTime - 0.7f;
		GetMesh()->bBlendPhysics = true;

		FTimerHandle DeathTimer;
		GetWorldTimerManager().SetTimer(DeathTimer, this, &AArenaCharacter::SetRagdollPhysics,
			TriggerRagdollTime, false);
	}
	else
	{
		SetRagdollPhysics();
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AArenaCharacter::GetAimLocationAndRotation(FVector& AimLocation, FVector& AimRotation) const
{	
	FRotator EyeRotation;
	GetActorEyesViewPoint(AimLocation, EyeRotation);
	AimRotation = EyeRotation.Vector();
}

void AArenaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindAxis("MoveForward", this, &AArenaCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AArenaCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &AArenaCharacter::Turn);
	InputComponent->BindAxis("LookUp", this, &AArenaCharacter::LookUp);
	InputComponent->BindAction("Reload", IE_Pressed, this, &AArenaCharacter::Reload);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AArenaCharacter::BeginSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AArenaCharacter::EndSprint);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AArenaCharacter::BeginJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AArenaCharacter::EndJump);
	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &AArenaCharacter::EquipNextWeapon);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AArenaCharacter::BeginFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AArenaCharacter::EndFire);
	InputComponent->BindAction("Ironsights", IE_Pressed, this, &AArenaCharacter::BeginIronsights);
	InputComponent->BindAction("Ironsights", IE_Released, this, &AArenaCharacter::EndIronsights);
	InputComponent->BindAction("ToggleView", IE_Pressed, this, &AArenaCharacter::ToggleView);
}

void AArenaCharacter::ServerSetSprinting_Implementation(bool bInSprinting)
{
	SetSprinting(bInSprinting);
}

bool AArenaCharacter::ServerSetSprinting_Validate(bool bInSprinting)
{
	return true;
}

void AArenaCharacter::ServerUpdateSpeed_Implementation()
{
	UpdateSpeed();
}

bool AArenaCharacter::ServerUpdateSpeed_Validate()
{
	return true;
}

void AArenaCharacter::ClientSetSprinting_Implementation(bool bInSprinting)
{
	SetSprinting(bInSprinting);
}

void AArenaCharacter::ServerUpdateStamina_Implementation(bool bDeplete)
{
	bDepleteStamina = bDeplete;
	bRegenStamina = !bDeplete;
}

bool AArenaCharacter::ServerUpdateStamina_Validate(bool bDeplete)
{
	return true;
}

void AArenaCharacter::ClientToggleView_Implementation()
{
	ToggleView();
}

void AArenaCharacter::OnRep_CurrentHealth()
{
	if (IsLocallyControlled())
	{
		UpdateHealthHUD();
	}
}

void AArenaCharacter::OnRep_CurrentStamina()
{
	if (IsLocallyControlled())
	{
		UpdateStaminaHUD();
	}
}

void AArenaCharacter::OnRep_CurrentShield()
{
	if (IsLocallyControlled())
	{
		UpdateShieldHUD();
	}
}

void AArenaCharacter::OnRep_CurrentSpeedMultiplier()
{
	UpdateSpeed();
}

void AArenaCharacter::OnRep_HitInfo()
{
	if (HitInfo.bKilled)
	{
		Die();
	}
}

void AArenaCharacter::ServerEquipWeapon_Implementation(int32 NextWeaponIndex)
{
	CurrentWeaponIndex = NextWeaponIndex;
}

bool AArenaCharacter::ServerEquipWeapon_Validate(int32 NextWeaponIndex)
{
	return true;
}

void AArenaCharacter::MulticastLookAtRotation_Implementation(FRotator InLookRotation)
{
	LookRotation = InLookRotation;
}

bool AArenaCharacter::MulticastLookAtRotation_Validate(FRotator InLookRotation)
{
	return true;
}