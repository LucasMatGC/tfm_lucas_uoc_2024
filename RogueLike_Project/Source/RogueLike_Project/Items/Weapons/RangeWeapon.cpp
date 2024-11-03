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
		
		TObjectPtr<ABaseProjectile> newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
			ProjectileType,
			FirePoint->GetComponentTransform(),
			//TODO: Definir como owner el player.
			nullptr,
			nullptr);

		SetupProjectile(newProjectile);		
		
		newProjectile->FinishSpawning(FirePoint->GetComponentTransform(), false, nullptr);

		CurrentMagazine--;
		CurrentAmmo--;
			
		FUpdateAmmo.Broadcast(CurrentMagazine, CurrentAmmo - CurrentMagazine);
	
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

void ARangeWeapon::SetupProjectile(TObjectPtr<ABaseProjectile> newProjectile)
{

	newProjectile->ProjectileComponent->ProjectileGravityScale = 0;
	newProjectile->ProjectileComponent->MaxSpeed = 1000.f;
	newProjectile->ProjectileComponent->InitialSpeed = 1000.f;
	newProjectile->InitialLifeSpan = 1.0f;
	
}
