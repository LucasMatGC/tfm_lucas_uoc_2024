// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "GameFramework/Character.h"
#include "RogueLike_Project/Objects/Weapons/MeleeWeapon.h"
#include "RogueLike_Project/Objects/Weapons/RangeWeapon.h"

// Sets default values
UInventoryComponent::UInventoryComponent()
{
	
}

// Called when the game starts or when spawned
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Initializes base variables of inventory, spawns and initializes weapons
void UInventoryComponent::SetupInventory(USceneComponent* newFirePoint)
{

	FirePoint = newFirePoint;
	
	for (TSubclassOf<ABaseWeapon> weaponType : WeaponTypes)
	{
		
		FActorSpawnParameters spawnInfo;
		FVector spawnLocation = FVector::ZeroVector;
		FRotator spawnRotation = FRotator::ZeroRotator;
		
		ABaseWeapon* weapon = GetWorld()->SpawnActor<ABaseWeapon>(weaponType, spawnLocation, spawnRotation, spawnInfo);
		if (ACharacter* character = Cast<ACharacter>(GetOwner()))
		{
			weapon->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
			weapon->FirePoint = FirePoint;
			weapon->SetUpWeapon(CommonUpgrades);
			weapon->DisableWeapon(true);
			Weapons.Add(weapon);
		}
		
	}
	Weapons[m_CurrentWeapon]->DisableWeapon(false);
}

// Change current weapon
void UInventoryComponent::ChangeWeapon(bool MoveForward)
{
	if (Weapons.Num() > 1)
	{

		Weapons[m_CurrentWeapon]->DisableWeapon(true);
		
		if (MoveForward)
		{
			m_CurrentWeapon == Weapons.Num() - 1 ? m_CurrentWeapon = 0 : m_CurrentWeapon++;
		}
		else
		{
			m_CurrentWeapon == 0 ? m_CurrentWeapon = Weapons.Num() - 1 : m_CurrentWeapon--;
		}

		Weapons[m_CurrentWeapon]->DisableWeapon(false);
	}
}

// Calls fire function of current weapon
void UInventoryComponent::FireCurrentWeapon()
{

	if (Weapons.IsValidIndex(m_CurrentWeapon))
	{
		Weapons[m_CurrentWeapon]->Fire();
		
	}
	
}

// Calls apply damage of current weapon if it's a melee weapon
void UInventoryComponent::ApplyMeleeDamage(AActor* OtherActor)
{

	if (AMeleeWeapon* CurrentWeapon = Cast<AMeleeWeapon>(Weapons[m_CurrentWeapon]))
	{

		CurrentWeapon->ApplyDamage(OtherActor);
		
	}
	
}

// Return current weapon
ABaseWeapon* UInventoryComponent::GetCurrentWeapon()
{

	if (Weapons.IsValidIndex(m_CurrentWeapon))
	{
		return Weapons[m_CurrentWeapon];
	}

	return  nullptr;
}

// Add ammo to all ranged weapons
void UInventoryComponent::AddAmmo(float ConsumableAmmo)
{

	for (ABaseWeapon* weapon : Weapons)
	{
		if (ARangeWeapon* rangedWeapon = Cast<ARangeWeapon>(weapon))
		{

			rangedWeapon->AddAmmo(ConsumableAmmo);
			
		}
	}

	if (ARangeWeapon* rangedWeapon = Cast<ARangeWeapon>(Weapons[m_CurrentWeapon]))
	{

		rangedWeapon->UpdateHUD();
			
	}
}

// Pauses game and show upgrade UI
void UInventoryComponent::PickUpItem(ABaseItem* NewPickedUpItem)
{

	PickedUpItem = NewPickedUpItem;
			
	if ( APlayerController* const player = Cast<APlayerController>(Cast<ACharacter>(GetOwner())->GetController()) )
	{
		player->SetPause(true);
	}

	OnUpgradeSelection.Broadcast(PickedUpItem, true);
	
}

// Attach the upgrade to the selected target
void UInventoryComponent::AttachPickedUpItem(int indexOfWeapon)
{
	// If player was the target, add upgrade to inventory and attach it to all weapons
	if (indexOfWeapon == -1)
	{
		FUpgradeStruct newUpgrade = PickedUpItem->DefaultUpgrade;
		CommonUpgrades.Add(newUpgrade);
		for (ABaseWeapon* weapon : Weapons)
		{
			weapon->AddUpgrade(newUpgrade, true);
		}
		OnUpgradeMaxHealth.Broadcast(newUpgrade.ExtraHealth);
	}
	// Else if a specific weapon was the target, add the specific upgrade to the weapon
	else if (indexOfWeapon >= 0 && indexOfWeapon <= Weapons.Num() - 1 )
	{
		FUpgradeStruct newUpgrade = PickedUpItem->DefaultUpgrade;
		if (PickedUpItem->UpgradeMapping.Contains(Weapons[indexOfWeapon]->GetClass()))
		{
			newUpgrade = *PickedUpItem->UpgradeMapping.Find(Weapons[indexOfWeapon]->GetClass());
			
		}
		
		Weapons[indexOfWeapon]->AddUpgrade(newUpgrade, false);
		OnUpgradeMaxHealth.Broadcast(newUpgrade.ExtraHealth);
	}
	
	PickedUpItem->Destroy();
	PickedUpItem = nullptr;

	// Resume game
	if ( APlayerController* const player = Cast<APlayerController>(Cast<ACharacter>(GetOwner())->GetController()) )
	{
		player->SetPause(false);
	}

	OnUpgradeSelection.Broadcast(nullptr, false);
	
	Weapons[m_CurrentWeapon]->UpdateHUD();
	
}

// Add upgrades to inventory and weapons on load level
void UInventoryComponent::AddUpgrade(FUpgradeStruct NewUpgrade, int indexOfWeapon)
{

	if (indexOfWeapon == -1)
	{
		
		CommonUpgrades.Add(NewUpgrade);
		for (ABaseWeapon* weapon : Weapons)
		{
			weapon->AddUpgrade(NewUpgrade, true);
		}
		OnUpgradeMaxHealth.Broadcast(NewUpgrade.ExtraHealth);
		
	}
	else if (indexOfWeapon >= 0 && indexOfWeapon < Weapons.Num())
	{
		Weapons[indexOfWeapon]->AddUpgrade(NewUpgrade, false);
		OnUpgradeMaxHealth.Broadcast(NewUpgrade.ExtraHealth);
	}
	
	Weapons[m_CurrentWeapon]->UpdateHUD();

	
}

// Returns common upgrades
TArray<FUpgradeStruct> UInventoryComponent::GetCommonUpgrades()
{

	return CommonUpgrades;
	
}
