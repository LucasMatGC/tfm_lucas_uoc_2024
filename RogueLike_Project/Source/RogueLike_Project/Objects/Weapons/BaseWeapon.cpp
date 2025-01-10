// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "RogueLike_Project/Objects/Items/Upgrades/UpgradeStruct.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootScene->SetupAttachment(RootComponent);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootScene);

	PrimaryActorTick.bCanEverTick = true;	
}

// Prepare weapon's common upgrades
void ABaseWeapon::SetUpWeapon(TArray<FUpgradeStruct> NewCommonUpgrades)
{

	CommonUpgrades = NewCommonUpgrades;
	
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Check if weapon can fire
bool ABaseWeapon::CanFire() const
{

	return true;
	
}

// Apply the upgrade
void ABaseWeapon::ApplyUpgrade(const FUpgradeStruct& Upgrade)
{
}

// Fire weapon
void ABaseWeapon::Fire()
{
}

// Reload Weapon
void ABaseWeapon::Reload()
{
}

// Update the weapon hud
void ABaseWeapon::UpdateHUD()
{
}

// Disable / Enable weapon
void ABaseWeapon::DisableWeapon(bool toHide) 
{
	// Hides visible components
	SetActorHiddenInGame(toHide);

	// Disables collision components
	SetActorEnableCollision(!toHide);

	// Stops the Actor from ticking
	SetActorTickEnabled(!toHide);

}

// Add upgrade to common or own list
void ABaseWeapon::AddUpgrade(FUpgradeStruct newUpgrade, bool bIsCommonUpgrade)
{

	if (bIsCommonUpgrade)
	{
		CommonUpgrades.Add(newUpgrade);
	}
	else
	{
		Upgrades.Add(newUpgrade);
	}
	
}
