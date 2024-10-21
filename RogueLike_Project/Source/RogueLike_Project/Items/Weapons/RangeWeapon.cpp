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
	
	if (WeaponMesh)
	{

		FirePoint = WeaponMesh->GetSocketByName("FirePoint");
		
	}
}

void ARangeWeapon::Fire()
{
	Super::Fire();

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, "Fire!!!!!!");

	if (CanFire())
	{
		
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, "FIRINGGGGG!!!!!!");
		
		FTransform projectileStartTransform;
		FirePoint->GetSocketTransform(projectileStartTransform, WeaponMesh);
		FActorSpawnParameters spawnInfo;
		
		TObjectPtr<ABaseProjectile> newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
			ProjectileType,
			projectileStartTransform,
			nullptr,
			nullptr);

		SetupProjectile(newProjectile);		
		
		newProjectile->FinishSpawning(projectileStartTransform, false, nullptr);

		CurrentMagazine--;
		CurrentAmmo--;
	}
}

void ARangeWeapon::Reload()
{

	if (CanReload())
	{

		CurrentAmmo >= MaxMagazine ? CurrentMagazine = MaxMagazine : CurrentMagazine = CurrentAmmo;
		
	}
	
}

bool ARangeWeapon::CanFire() const
{

	return CurrentMagazine > 0 && FirePoint != nullptr;
	
}

bool ARangeWeapon::CanReload() const
{

	return CurrentMagazine < MaxMagazine && CurrentMagazine < CurrentAmmo;
	
}

void ARangeWeapon::SetupProjectile(TObjectPtr<ABaseProjectile> newProjectile)
{

	newProjectile->ProjectileComponent->ProjectileGravityScale = 0;
	newProjectile->ProjectileComponent->MaxSpeed = 300.f;
	newProjectile->ProjectileComponent->InitialSpeed = 300.f;
	
}
