// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "RogueLike_Project/Objects/Items/BaseItem.h"

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

// Called every frame
void ARangeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Decrease fire delay if possible
	if (CurrentFireRate > 0)
	{

		CurrentFireRate -= DeltaTime;
		
	}
	
}

// Fire weapon
void ARangeWeapon::Fire()
{
	Super::Fire();

	// If Weapon can fire, spawn projectile
	if (CanFire())
	{

		CurrentFireRate = MaxFireRate;
		
		FActorSpawnParameters spawnInfo;

		// Partially Spawn projectile 
		newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
			ProjectileType,
			FirePoint->GetComponentTransform(),
			nullptr,
			nullptr);

		// Setup projectile with upgrade modifications
		SetupProjectile();		

		// Finish spawning projectile
		newProjectile->FinishSpawning(FirePoint->GetComponentTransform(), false, nullptr);

		// Decrease ammo
		CurrentMagazine--;
		CurrentAmmo--;
			
		UpdateHUD();
		UGameplayStatics::SpawnSound2D(this, FireSFX);
		
		OnWeaponFire.Broadcast(false, 0);
	}
	// If no ammo to fire, show feedback
	else if (CurrentMagazine == 0)
	{
		OnEmptyMagazineShoot.Broadcast();
	}
}

// Reload weapon
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

// If current magazine has bullets and theres no fire cooldown, return true
bool ARangeWeapon::CanFire() const
{
	
	return CurrentMagazine > 0 && CurrentFireRate <= 0;
	
}

// If there are bullets outside of current magazine, return true
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

// Add ammo to weapon capped to MaxAmmo value
void ARangeWeapon::AddAmmo(float ConsumableAmmo)
{

	(CurrentAmmo + ConsumableAmmo) >= MaxAmmo ? CurrentAmmo = MaxAmmo : CurrentAmmo += ConsumableAmmo;
	
}

void ARangeWeapon::UpdateHUD()
{

	FUpdateAmmo.Broadcast(CurrentMagazine, CurrentAmmo - CurrentMagazine);
	
}

// Add upgrade to weapon
void ARangeWeapon::AddUpgrade(FUpgradeStruct newUpgrade, bool bIsCommonUpgrade)
{
	Super::AddUpgrade(newUpgrade, bIsCommonUpgrade);

	// If upgrade changes base variables, add values
	if (newUpgrade.UpgradeType == EUpgradeType::BaseVariablesUpgrade || newUpgrade.UpgradeType != EUpgradeType::CustomizedUpgrade)
	{

		AddedDamage = FMath::Max(AddedDamage + newUpgrade.AddedDamage, (1 - BaseDamage));
		DamageMultiplier = FMath::Max(DamageMultiplier + newUpgrade.AddedDamageMultiplier, 0.1f);
		Range = FMath::Max(Range + newUpgrade.AddedRange, 100.0f);
		MaxFireRate = FMath::Max(MaxFireRate - newUpgrade.ReducedFireRate, 0.1f);
		MaxAmmo = FMath::Max(MaxAmmo + newUpgrade.AddedMaxAmmo, 1);
		ExtraProjectileScale = FMath::Max(ExtraProjectileScale + newUpgrade.ExtraScale, 0.0);
		MaxMagazine = FMath::Max(MaxMagazine + newUpgrade.AddedMaxMagazine, 1);

	}

	// If upgrade is a modifiear, change modifier values
	if (newUpgrade.UpgradeType != EUpgradeType::ModifierUpgrade || newUpgrade.UpgradeType != EUpgradeType::CustomizedUpgrade)
	{
		LifeSteal += FMath::Max(newUpgrade.LifeSteal, 0.0f);
	}
	
}

// Apply upgrade to projectile
void ARangeWeapon::ApplyUpgrade(const FUpgradeStruct& Upgrade)
{
	// If upgrade is a modifier or custom
	if (Upgrade.UpgradeType == EUpgradeType::ModifierUpgrade || Upgrade.UpgradeType == EUpgradeType::CustomizedUpgrade)
	{

		// If there is a new mesh, apply it
		if (Upgrade.NewProjectileMesh != nullptr)
		{
			newProjectile->ProjectileMesh->SetStaticMesh(Upgrade.NewProjectileMesh);
		}

		// Change Bounciness if needed
		if (Upgrade.Bounciness > 0.0f)
		{
			
			newProjectile->ProjectileComponent->Bounciness = Upgrade.Bounciness;
			newProjectile->ProjectileComponent->bShouldBounce = true;
			
		}

		// Change collision profile
		if (Upgrade.ChangeCollisionProfile)
		{
			newProjectile->ProjectileMesh->SetCollisionProfileName(Upgrade.NewCollisionProfile.Name, true);
		}
		
		// Change projectile destruction on impcat
		if (Upgrade.ChangeProjectileDestruction)
		{
			newProjectile->bDestroyOnImpact = Upgrade.DestroyOnImpact;
		}
		
	}
	
	
}

// Setup projectile with upgrades
void ARangeWeapon::SetupProjectile()
{
	
	newProjectile->ProjectileComponent->MaxSpeed = Range;
	newProjectile->ProjectileComponent->InitialSpeed = Range;
	newProjectile->InitialLifeSpan = 1.0f;
	newProjectile->BaseDamage = BaseDamage;
	newProjectile->AddedDamage = AddedDamage;
	newProjectile->DamageMultiplier = DamageMultiplier;
	newProjectile->LifeSteal = LifeSteal;
	newProjectile->OwnerController = GetWorld()->GetFirstPlayerController();
	FVector NewScale = FVector(
		newProjectile->ProjectileMesh->GetRelativeScale3D().X + ExtraProjectileScale,
		newProjectile->ProjectileMesh->GetRelativeScale3D().Y + ExtraProjectileScale,
		newProjectile->ProjectileMesh->GetRelativeScale3D().Z + ExtraProjectileScale);
	newProjectile->ProjectileMesh->SetRelativeScale3D(NewScale);

	// Apply common upgrades
	for (FUpgradeStruct commonUpgrade : CommonUpgrades)
	{

		ApplyUpgrade(commonUpgrade);
		
	}

	// Apply weapon upgrades
	for (FUpgradeStruct upgrade : Upgrades)
	{

		ApplyUpgrade(upgrade);
		
	}
	
	
}
