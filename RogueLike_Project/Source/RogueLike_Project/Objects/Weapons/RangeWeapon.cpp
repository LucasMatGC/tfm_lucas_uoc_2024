// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeapon.h"

// Sets default values
ARangeWeapon::ARangeWeapon()
{

	MaxAmmo = MaxMagazine = CurrentAmmo = CurrentMagazine = 0;
	
}

// Called when the game starts or when spawned
void ARangeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARangeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentFireRate > 0)
	{

		CurrentFireRate -= DeltaTime;
		
	}
	
}

void ARangeWeapon::Fire()
{
	Super::Fire();

	if (CanFire())
	{

		CurrentFireRate = MaxFireRate;
		
		FActorSpawnParameters spawnInfo;
		
		newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
			ProjectileType,
			FirePoint->GetComponentTransform(),
			nullptr,
			nullptr);

		SetupProjectile();		
		
		newProjectile->FinishSpawning(FirePoint->GetComponentTransform(), false, nullptr);

		CurrentMagazine--;
		CurrentAmmo--;
			
		UpdateHUD();
		
		OnWeaponFire.Broadcast(false, 0);
	}
}

void ARangeWeapon::Reload()
{

	if (CanReload())
	{

		CurrentAmmo >= MaxMagazine ? CurrentMagazine = MaxMagazine : CurrentMagazine = CurrentAmmo;
		
	}
	
}

int ARangeWeapon::GetMaxAmmo()
{

	return MaxAmmo;
	
}

int ARangeWeapon::GetMaxMagazine()
{

	return MaxMagazine;
	
}

bool ARangeWeapon::CanFire() const
{

	return CurrentMagazine > 0 && CurrentFireRate <= 0;
	
}

bool ARangeWeapon::CanReload()
{

	return CurrentMagazine < MaxMagazine && CurrentMagazine < CurrentAmmo;
	
}

int ARangeWeapon::GetCurrentAmmo()
{

	return CurrentAmmo;
	
}

int ARangeWeapon::GetCurrentMagazine()
{

	return CurrentMagazine;
	
}

void ARangeWeapon::AddAmmo(float ConsumableAmmo)
{

	(CurrentAmmo + ConsumableAmmo) >= MaxAmmo ? CurrentAmmo = MaxAmmo : CurrentAmmo += ConsumableAmmo;
	
}

void ARangeWeapon::UpdateHUD()
{

	FUpdateAmmo.Broadcast(CurrentMagazine, CurrentAmmo - CurrentMagazine);
	
}

void ARangeWeapon::AddUpgrade(FUpgradeStruct newUpgrade, bool bIsCommonUpgrade)
{
	Super::AddUpgrade(newUpgrade, bIsCommonUpgrade);

	AddedDamage += newUpgrade.AddedDamage;
	DamageMultiplier += newUpgrade.AddedDamageMultiplier;
	Range += newUpgrade.AddedRange;
	MaxFireRate -= newUpgrade.ReducedFireRate;
	MaxAmmo += newUpgrade.AddedMaxAmmo;
	MaxMagazine += newUpgrade.AddedMaxMagazine;
	
}

void ARangeWeapon::ApplyUpgrade(const FUpgradeStruct& Upgrade)
{
	
	switch (Upgrade.UpgradeType)
	{

		case EUpgradeType::DamageTypeUpgrade:

			newProjectile->DamageType = Upgrade.DamageType;
			break;
				
		case EUpgradeType::ModifierUpgrade:

			newProjectile->ProjectileMesh->SetStaticMesh(Upgrade.NewProjectileMesh);
			break;
			
		default:
			break;
	}
	
}

void ARangeWeapon::SetupProjectile()
{
	
	newProjectile->ProjectileComponent->MaxSpeed = 1000.f;
	newProjectile->ProjectileComponent->InitialSpeed = 1000.f;
	newProjectile->InitialLifeSpan = 1.0f;
	newProjectile->BaseDamage = BaseDamage;
	newProjectile->AddedDamage = AddedDamage;
	newProjectile->DamageMultiplier = DamageMultiplier;
	newProjectile->OwnerController = GetWorld()->GetFirstPlayerController();
	
	for (FUpgradeStruct commonUpgrade : CommonUpgrades)
	{

		ApplyUpgrade(commonUpgrade);
		
	}
	
	for (FUpgradeStruct upgrade : Upgrades)
	{

		ApplyUpgrade(upgrade);
		
	}
	
	
}
