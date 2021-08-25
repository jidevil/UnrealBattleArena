// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/ArenaWeapon.h"
#include "Weapons/ArenaWeaponProjectile.h"
#include "Character/ArenaCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UnrealBattleArena.h"
#include "Player/ArenaPlayerController.h"
#include "UI/HUD/ArenaHUD.h"
#include "UI/HUD/Widgets/ArenaWeaponWidget.h"

AArenaWeapon::AArenaWeapon()
{
	bReplicates = true;
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh1P");
	Mesh1P->SetHiddenInGame(true);
	Mesh1P->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	RootComponent = Mesh1P;

	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh3P");
	Mesh3P->SetupAttachment(Mesh1P);
	Mesh3P->SetHiddenInGame(true);
	Mesh3P->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
}

USkeletalMeshComponent* AArenaWeapon::GetMesh() const
{
	return ArenaCharacter->IsFirstPersonView() ? Mesh1P : Mesh3P;
}

const FName& AArenaWeapon::GetInventorySocket() const
{
	return WeaponInventorySocket;
}

const FWeaponAttributes& AArenaWeapon::GetAttributes() const
{
	return Attributes;
}

AArenaCharacter* AArenaWeapon::GetCharacter() const
{
	return ArenaCharacter;
}

bool AArenaWeapon::HasLeftHandIK() const
{
	return bHasLeftHandIK;
}

bool AArenaWeapon::IsInIronsights() const
{
	return bIronsights;
}

bool AArenaWeapon::IsReady() const
{
	return !(bEquipping || bUnEquipping || bIronsights || bReloading);
}

bool AArenaWeapon::CanEquip() const
{
	const bool bReady = !(bEquipping || bUnEquipping || bIronsights);
	return bReady;
}

bool AArenaWeapon::CanUnEquip() const
{
	const bool bReady = !(bEquipping || bUnEquipping || bIronsights);
	return bReady;
}

bool AArenaWeapon::CanReload() const
{
	const bool bReady = !(bEquipping || bUnEquipping || bReloading);
	const bool bNeedsReload = CurrentAmmoInClip < Attributes.AmmoPerClip;
	const bool bHasAmmo = RemainingAmmo > 0;
	
	return bReady && bNeedsReload && bHasAmmo;
}

bool AArenaWeapon::CanFire() const
{
	const bool bReady = !(bEquipping || bReloading) && FireCount == 0;
	const bool bHasAmmo = CurrentAmmoInClip > 0 || RemainingAmmo > 0;
	
	return bReady && bHasAmmo;
}

bool AArenaWeapon::CanIronsights() const
{
	const bool bReady = !(bEquipping || bReloading || bIronsights);
	return bReady;
}

bool AArenaWeapon::CanAddClip() const
{
	return (Attributes.MaxAmmoCount - RemainingAmmo) > 0;
}

int32 AArenaWeapon::GetRemainingAmmo() const
{
	return RemainingAmmo;
}

void AArenaWeapon::AttachToPlayer()
{
	if (!bEquipped)
	{
		if (!ArenaCharacter->IsFirstPersonView())
		{
			const FAttachmentTransformRules& AttachmentRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
			Mesh3P->AttachToComponent(ArenaCharacter->GetMesh(), AttachmentRules,
				WeaponInventorySocket);

			Mesh3P->SetHiddenInGame(false);
			Mesh1P->SetHiddenInGame(true);
		}
		else
		{			
			Mesh3P->SetHiddenInGame(true);
			Mesh1P->SetHiddenInGame(true);
		}
	}
	else
	{
		const FAttachmentTransformRules& AttachmentRules = FAttachmentTransformRules::KeepRelativeTransform;
		GetMesh()->AttachToComponent(ArenaCharacter->GetCharacterMesh(), AttachmentRules,
			bIronsights ? WeaponIronsightSocket : WeaponSocket);
		
		if (!ArenaCharacter->IsFirstPersonView())
		{
			Mesh3P->SetHiddenInGame(false);
			Mesh1P->SetHiddenInGame(true);
		}
		else
		{
			Mesh3P->SetHiddenInGame(true);
			Mesh1P->SetHiddenInGame(false);
		}
	}
}

void AArenaWeapon::AddClips(int32 ClipCount)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		if (ArenaCharacter->IsLocallyControlled())
		{
			ServerAddClips(ClipCount);
		}
	}
	else
	{
		int32 NewAmmoCount = ClipCount * Attributes.AmmoPerClip;
		const int32 NewTotalAmmoCount = CurrentAmmoInClip + RemainingAmmo + NewAmmoCount;
	
		if (NewTotalAmmoCount > Attributes.MaxAmmoCount)
		{
			NewAmmoCount = Attributes.MaxAmmoCount - RemainingAmmo;
		}
	
		RemainingAmmo += NewAmmoCount;
		if (bEquipped && CurrentAmmoInClip == 0)
		{
			ClientReload();
		}
	}
}

void AArenaWeapon::ToggleView()
{
	if (GetLocalRole() < ROLE_Authority && ArenaCharacter->IsLocallyControlled())
	{
		ServerToggleView();
	}

	bToggleView = !bToggleView;
	AttachToPlayer();
}

void AArenaWeapon::Equip()
{	
	if (ArenaCharacter->IsLocallyControlled())
	{
		StopReload();
		StopFire();

		if (GetLocalRole() < ROLE_Authority)
		{
			ServerEquip();
		}
	}
	
	bHasLeftHandIK = false;
	bEquipping = true;
	
	const FWeaponCharacterAnimations& CharacterAnimations = ArenaCharacter->IsFirstPersonView() ?
		CharacterAnimations1P : CharacterAnimations3P;
	
	const float AnimTime = ArenaCharacter->PlayAnimMontage(CharacterAnimations.Equip);
	const float EquipTime = AnimTime / 2.0f;
	
	GetWorldTimerManager().SetTimer(EquipTimer, this, &AArenaWeapon::OnEquip,
		EquipTime, false);
	
	GetWorldTimerManager().SetTimer(EquipFinishedTimer, this, &AArenaWeapon::OnEquipFinished,
		AnimTime, false);
}

void AArenaWeapon::UnEquip()
{
	if (ArenaCharacter->IsLocallyControlled())
	{
		StopReload();
		StopFire();

		if (GetLocalRole() < ROLE_Authority)
		{
			ServerUnEquip();
		}
	}

	bUnEquipping = true;

	const FWeaponCharacterAnimations& CharacterAnimations = ArenaCharacter->IsFirstPersonView() ?
		CharacterAnimations1P : CharacterAnimations3P;
	
	const float AnimTime = CharacterAnimations.Equip->GetPlayLength();
	const float UnEquipTime = AnimTime / 2.0f;

	GetWorldTimerManager().SetTimer(UnEquipTimer, this, &AArenaWeapon::OnUnEquip,
		UnEquipTime, false);
	
	GetWorldTimerManager().SetTimer(UnEquipFinishedTimer, this, &AArenaWeapon::OnUnEquipFinished,
		AnimTime, false);
}

void AArenaWeapon::Reload()
{
	if (ArenaCharacter->IsLocallyControlled())
	{
		StopFire();

		if (GetLocalRole() < ROLE_Authority)
		{
			ServerReload();
		}
	}

	bReloading = true;
	bHasLeftHandIK = false;
	
	const FWeaponCharacterAnimations& CharacterAnimations = ArenaCharacter->IsFirstPersonView() ?
		CharacterAnimations1P : CharacterAnimations3P;
	
	const float AnimTime = ArenaCharacter->PlayAnimMontage(CharacterAnimations.Reload);

	GetWorldTimerManager().SetTimer(ReloadTimer, this, &AArenaWeapon::OnReload,
		AnimTime - 0.7f, false);

	GetWorldTimerManager().SetTimer(ReloadFinishedTimer, this, &AArenaWeapon::OnReloadFinished,
		AnimTime, false);
	
	if (Animations.Reload)
	{
		GetMesh()->PlayAnimation(Animations.Reload, false);
	}
}

void AArenaWeapon::BeginFire()
{
	OnFire();	
	GetWorldTimerManager().SetTimer(FireFinishedTimer, this, &AArenaWeapon::OnFireFinished,
		Attributes.ShotDuration, false);
}

void AArenaWeapon::EndFire()
{
}

void AArenaWeapon::BeginIronsights()
{
	if (GetLocalRole() < ROLE_Authority && ArenaCharacter->IsLocallyControlled())
	{
		ArenaCharacter->SetCameraFOV(Attributes.IronsightsFOV);
		ServerBeginIronsights();
	}
	
	bIronsights = true;
	AttachToPlayer();
}

void AArenaWeapon::EndIronsights()
{
	if (GetLocalRole() < ROLE_Authority && ArenaCharacter->IsLocallyControlled())
	{
		ArenaCharacter->SetCameraFOV(CHARACTER_DEFAULT_FOV);
		ServerEndIronsights();
	}
	
	bIronsights = false;

	FTimerDelegate EndIronsightsCallback;
	EndIronsightsCallback.BindLambda([this]
	{
		AttachToPlayer();
	});

	FTimerHandle EndIronsightsTimer;
	GetWorldTimerManager().SetTimer(EndIronsightsTimer, EndIronsightsCallback, 0.1f, false);
}

void AArenaWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() < ROLE_Authority)
	{
		AArenaPlayerController* PlayerController = Cast<AArenaPlayerController>(
			GetWorld()->GetFirstPlayerController());

		if (PlayerController)
		{
			UArenaHUD* PlayerHUD = PlayerController->GetPlayerHUD();
			if (PlayerHUD)
			{
				WeaponWidget = PlayerHUD->GetWeaponWidget();
			}
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		ArenaCharacter = Cast<AArenaCharacter>(GetOwner());
	}
}

void AArenaWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArenaWeapon, CurrentAmmoInClip);
	DOREPLIFETIME(AArenaWeapon, RemainingAmmo);
	DOREPLIFETIME(AArenaWeapon, ArenaCharacter);
	DOREPLIFETIME_CONDITION(AArenaWeapon, bToggleView, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AArenaWeapon, bEquipped, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AArenaWeapon, bEquipping, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AArenaWeapon, bUnEquipping, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AArenaWeapon, bReloading, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AArenaWeapon, FireCount, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AArenaWeapon, bIronsights, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AArenaWeapon, HitInfo, COND_SkipOwner);
}

void AArenaWeapon::StopReload()
{
	if (GetLocalRole() < ROLE_Authority && ArenaCharacter->IsLocallyControlled())
	{
		ServerStopReload();
	}

	bReloading = false;
	
	const FWeaponCharacterAnimations& CharacterAnimations = ArenaCharacter->IsFirstPersonView() ?
		CharacterAnimations1P : CharacterAnimations3P;

	ArenaCharacter->StopAnimMontage(CharacterAnimations.Reload);

	GetWorldTimerManager().ClearTimer(ReloadTimer);
	GetWorldTimerManager().ClearTimer(ReloadFinishedTimer);
}

void AArenaWeapon::StopFire()
{
	if (GetLocalRole() < ROLE_Authority && ArenaCharacter->IsLocallyControlled())
	{
		ServerStopFire();
	}

	FireCount = 0;
	
	const FWeaponCharacterAnimations& CharacterAnimations = ArenaCharacter->IsFirstPersonView() ?
		CharacterAnimations1P : CharacterAnimations3P;

	if (bIronsights)
	{
		ArenaCharacter->StopAnimMontage(CharacterAnimations.FireIronsights);
	}
	else
	{
		ArenaCharacter->StopAnimMontage(CharacterAnimations.Fire);
	}

	GetWorldTimerManager().ClearTimer(FireFinishedTimer);
}

void AArenaWeapon::Fire(const FVector& AimLocation, const FVector& AimRotation)
{
	const float HalfAngle = FMath::DegreesToRadians(bIronsights ? Attributes.IronsightsBulletSpread :
		Attributes.BulletSpread);
	const FVector TraceRotation = FMath::VRandCone(AimRotation, HalfAngle,
		HalfAngle);
			
	const FVector TraceStart = AimLocation;
	const FVector TraceEnd = TraceStart + TraceRotation * Attributes.BulletRange;

	const FHitResult HitResult = WeaponTrace(TraceStart, TraceEnd);
	const FVector ProjEnd = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;

	const FVector ProjStart = GetMesh()->GetSocketLocation(MuzzleSocket);
	const FRotator ProjRotation = UKismetMathLibrary::FindLookAtRotation(ProjStart, ProjEnd);

	FTransform MuzzleTransform = GetMesh()->GetSocketTransform(MuzzleSocket);
	MuzzleTransform.SetRotation(ProjRotation.Quaternion());

	AArenaWeaponProjectile* Projectile = GetWorld()->SpawnActorDeferred<AArenaWeaponProjectile>(ProjectileClass, MuzzleTransform,
		this, ArenaCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (Projectile)
	{
		Projectile->FinishSpawning(MuzzleTransform);
	}
	
	if (!Attributes.bProjectileDamage)
	{		
		if (HitResult.bBlockingHit)
		{
			SpawnImpactEffects(HitResult);	
		}

		ApplyDamage(HitResult);
	}
}

void AArenaWeapon::OnEquip()
{
	bEquipped = true;
	
	AttachToPlayer();
	UpdateAmmoHUD();
}

void AArenaWeapon::OnEquipFinished()
{
	bEquipping = false;
	bHasLeftHandIK = true;

	if (GetLocalRole() == ROLE_Authority)
	{
		if (CurrentAmmoInClip == 0 && RemainingAmmo > 0)
		{
			ClientReload();
		}
	}
}

void AArenaWeapon::OnUnEquip()
{
	bEquipped = false;
	AttachToPlayer();
}

void AArenaWeapon::OnUnEquipFinished()
{
	bUnEquipping = false;
	bHasLeftHandIK = true;
}

void AArenaWeapon::OnReload()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		const int32 AmmoToAdd = FMath::Min(Attributes.AmmoPerClip - CurrentAmmoInClip,
			RemainingAmmo);
		
		CurrentAmmoInClip += AmmoToAdd;
		RemainingAmmo -= AmmoToAdd;
	}
}

void AArenaWeapon::OnReloadFinished()
{
	bReloading = false;
	bHasLeftHandIK = true;
}

void AArenaWeapon::OnFire()
{
	FVector AimLocation = HitInfo.AimLocation;
	FVector AimRotation = HitInfo.AimRotation;
	
	if (ArenaCharacter->IsLocallyControlled())
	{
		ArenaCharacter->GetAimLocationAndRotation(AimLocation, AimRotation);
		
		if (GetLocalRole() < ROLE_Authority)
		{
			ServerOnFire(AimLocation, AimRotation);
		}
		else
		{
			HitInfo.AimLocation = AimLocation;
			HitInfo.AimRotation = AimRotation;
		}
	}
	
	if (CurrentAmmoInClip == 0)
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			if (CanReload())
			{
				ClientStopFire();
				ClientReload();
			}

			return;
		}

		if (ArenaCharacter->IsLocallyControlled())
		{
			return;
		}
	}

	FireCount++;
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentAmmoInClip--;
	}

	const FWeaponCharacterAnimations& CharacterAnimations = ArenaCharacter->IsFirstPersonView() ?
		CharacterAnimations1P : CharacterAnimations3P;

	ArenaCharacter->PlayAnimMontage(bIronsights ? CharacterAnimations.FireIronsights :
		CharacterAnimations.Fire);
	GetMesh()->PlayAnimation(Animations.Fire, false);
	
	Fire(AimLocation, AimRotation);
}

void AArenaWeapon::OnFireFinished()
{
	StopFire();
}

void AArenaWeapon::SpawnImpactEffects(const FHitResult& HitResult)
{
	const EPhysicalSurface PhysicalSurface = UPhysicalMaterial::DetermineSurfaceType(
		HitResult.PhysMaterial.Get());

	if (ImpactEffects.Contains(PhysicalSurface))
	{
		const FWeaponImpactEffect& ImpactEffect = ImpactEffects[PhysicalSurface];
		if (ImpactEffect.Particle)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(HitResult.ImpactPoint);
			SpawnTransform.SetRotation(HitResult.ImpactNormal.ToOrientationQuat());

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect.Particle, SpawnTransform);
		}

		if (ImpactEffect.Sound)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactEffect.Sound, HitResult.ImpactPoint,
				HitResult.ImpactNormal.ToOrientationRotator());
		}
	}
}

void AArenaWeapon::ApplyDamage(const FHitResult& HitResult)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (HitResult.GetActor() && HitResult.bBlockingHit)
		{
			float TotalDamage = DamageAttributes.DefaultDamage;
			const EPhysicalSurface PhysicalSurface = UPhysicalMaterial::DetermineSurfaceType(
			HitResult.PhysMaterial.Get());

			switch (PhysicalSurface)
			{
			case SURFACE_TYPE_HEAD:
				{
					TotalDamage *= DamageAttributes.HeadshotMultiplier;
					break;
				}

			case SURFACE_TYPE_BODY:
				{
					TotalDamage *= DamageAttributes.BodyshotMultiplier;
					break;
				}

			case SURFACE_TYPE_FOOT:
				{
					TotalDamage *= DamageAttributes.FootshotMultiplier;
					break;
				}

			default:
				break;
			}

			UGameplayStatics::ApplyDamage(HitResult.GetActor(), TotalDamage, ArenaCharacter->GetController(),
				ArenaCharacter, nullptr);
		}
	}
}

FHitResult AArenaWeapon::WeaponTrace(const FVector& TraceStart, const FVector& TraceEnd)
{
	FCollisionQueryParams QueryParams;
	QueryParams.TraceTag = SCENE_QUERY_STAT(WeaponTrace);
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(ArenaCharacter);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FHitResult HitResult{ ForceInit };
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
		COLLISION_WEAPON, QueryParams);

	return HitResult;
}

void AArenaWeapon::UpdateAmmoHUD()
{
	if (WeaponWidget)
	{
		WeaponWidget->SetAmmo(CurrentAmmoInClip, RemainingAmmo);
	}
}

void AArenaWeapon::ServerToggleView_Implementation()
{
	ToggleView();
}

bool AArenaWeapon::ServerToggleView_Validate()
{
	return true;
}

void AArenaWeapon::ServerEquip_Implementation()
{
	Equip();
}

bool AArenaWeapon::ServerEquip_Validate()
{
	return true;
}

void AArenaWeapon::ServerUnEquip_Implementation()
{
	UnEquip();
}

bool AArenaWeapon::ServerUnEquip_Validate()
{
	return true;
}

void AArenaWeapon::ServerReload_Implementation()
{
	Reload();
}

bool AArenaWeapon::ServerReload_Validate()
{
	return true;
}

void AArenaWeapon::ServerBeginIronsights_Implementation()
{
	BeginIronsights();
}

bool AArenaWeapon::ServerBeginIronsights_Validate()
{
	return true;
}

void AArenaWeapon::ServerEndIronsights_Implementation()
{
	EndIronsights();
}

bool AArenaWeapon::ServerEndIronsights_Validate()
{
	return true;
}

void AArenaWeapon::ServerStopReload_Implementation()
{
	StopReload();
}

bool AArenaWeapon::ServerStopReload_Validate()
{
	return true;
}

void AArenaWeapon::ServerStopFire_Implementation()
{
	StopFire();
}

bool AArenaWeapon::ServerStopFire_Validate()
{
	return true;
}

void AArenaWeapon::ServerAddClips_Implementation(int32 ClipCount)
{
	AddClips(ClipCount);
}

bool AArenaWeapon::ServerAddClips_Validate(int32 ClipCount)
{
	return true;
}

void AArenaWeapon::ServerOnFire_Implementation(const FVector& AimLocation, const FVector& AimRotation)
{
	HitInfo.AimLocation = AimLocation;
	HitInfo.AimRotation = AimRotation;
	
	OnFire();
}

bool AArenaWeapon::ServerOnFire_Validate(const FVector& AimLocation, const FVector& AimRotation)
{
	return true;
}

void AArenaWeapon::ClientReload_Implementation()
{
	Reload();
}

void AArenaWeapon::ClientStopFire_Implementation()
{
	StopFire();
}

void AArenaWeapon::OnRep_ToggleView()
{
	ToggleView();
}	

void AArenaWeapon::OnRep_Ironsights()
{
	if (bIronsights)
	{
		BeginIronsights();
	}
	else
	{
		EndIronsights();
	}
}

void AArenaWeapon::OnRep_Equipping()
{
	if (bEquipping)
	{
		Equip();
	}
}

void AArenaWeapon::OnRep_UnEquipping()
{
	if (bUnEquipping)
	{
		UnEquip();
	}
}

void AArenaWeapon::OnRep_Reloading()
{
	if (bReloading)
	{
		Reload();
	}
	else
	{
		StopReload();
	}
}

void AArenaWeapon::OnRep_FireCount()
{
	if (FireCount == 0)
	{
		StopFire();
	}
	else
	{
		OnFire();
	}
}

void AArenaWeapon::OnRep_CurrentAmmoInClip()
{
	if (ArenaCharacter->IsLocallyControlled())
	{
		UpdateAmmoHUD();
	}
}

void AArenaWeapon::OnRep_RemainingAmmo()
{
	if (ArenaCharacter->IsLocallyControlled())
	{
		UpdateAmmoHUD();
	}
}
