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
		UGameplayStatics::SpawnSound2D(this, FireSFX);
		
		OnWeaponFire.Broadcast(false, 0);
	}
	else if (CurrentMagazine == 0)
	{
		OnEmptyMagazineShoot.Broadcast();
		UGameplayStatics::SpawnSound2D(this, EmptyMagazineSFX);
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

	if (newUpgrade.UpgradeType == EUpgradeType::BaseVariablesUpgrade || newUpgrade.UpgradeType != EUpgradeType::CustomizedUpgrade)
	{

		//TODO: Cambiar estas asignaciones. Ahora mismo se esta sumando el maximo de lo que define el upgrade y el valor de corte, cuando lo que deberia de caparse es la suma ya hecha
		AddedDamage = FMath::Max(AddedDamage + newUpgrade.AddedDamage, (1 - BaseDamage));
		DamageMultiplier = FMath::Max(DamageMultiplier + newUpgrade.AddedDamageMultiplier, 0.1f);
		Range = FMath::Max(Range + newUpgrade.AddedRange, 100.0f);
		MaxFireRate = FMath::Max(MaxFireRate - newUpgrade.ReducedFireRate, 0.1f);
		MaxAmmo = FMath::Max(MaxAmmo + newUpgrade.AddedMaxAmmo, 1);
		ExtraProjectileScale = FMath::Max(ExtraProjectileScale + newUpgrade.ExtraScale, 0.0);
		MaxMagazine = FMath::Max(MaxMagazine + newUpgrade.AddedMaxMagazine, 1);

	}

	if (newUpgrade.UpgradeType != EUpgradeType::ModifierUpgrade || newUpgrade.UpgradeType != EUpgradeType::CustomizedUpgrade)
	{
		LifeSteal += FMath::Max(newUpgrade.LifeSteal, 0.0f);
	}
	
}

void ARangeWeapon::ApplyUpgrade(const FUpgradeStruct& Upgrade)
{
	
	if (Upgrade.UpgradeType == EUpgradeType::ModifierUpgrade || Upgrade.UpgradeType == EUpgradeType::CustomizedUpgrade)
	{

		if (Upgrade.NewProjectileMesh != nullptr)
		{
			newProjectile->ProjectileMesh->SetStaticMesh(Upgrade.NewProjectileMesh);
		}

		if (Upgrade.Bounciness > 0.0f)
		{
			
			newProjectile->ProjectileComponent->Bounciness = Upgrade.Bounciness;
			newProjectile->ProjectileComponent->bShouldBounce = true;
			
		}
		
		if (Upgrade.ChangeCollisionProfile)
		{
			newProjectile->ProjectileMesh->SetCollisionProfileName(Upgrade.NewCollisionProfile.Name, true);
		}
		//If the projectile is already set as not destroy on impact, avoid setting this flag. It will revert it if
		//the order of upgrades is changed
		if (Upgrade.ChangeProjectileDestruction)
		{
			newProjectile->bDestroyOnImpact = Upgrade.DestroyOnImpact;
		}
		
	}
	
	
}

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
	
	for (FUpgradeStruct commonUpgrade : CommonUpgrades)
	{

		ApplyUpgrade(commonUpgrade);
		
	}
	
	for (FUpgradeStruct upgrade : Upgrades)
	{

		ApplyUpgrade(upgrade);
		
	}
	
	
}
