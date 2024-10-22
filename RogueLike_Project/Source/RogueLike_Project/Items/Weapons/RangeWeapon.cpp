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
		
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "FIRINGGGGG!!!!!!");
		
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

	return CurrentMagazine > 0 && FirePoint != nullptr && CurrentFireRate <= 0;
	
}

bool ARangeWeapon::CanReload()
{

	return CurrentMagazine < MaxMagazine && CurrentMagazine < CurrentAmmo;
	
}

void ARangeWeapon::SetupProjectile(TObjectPtr<ABaseProjectile> newProjectile)
{

	newProjectile->ProjectileComponent->ProjectileGravityScale = 0;
	newProjectile->ProjectileComponent->MaxSpeed = 1000.f;
	newProjectile->ProjectileComponent->InitialSpeed = 1000.f;
	newProjectile->InitialLifeSpan = 1.0f;
	
}
