// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "GameFramework/Character.h"
#include "RogueLike_Project/Objects/Weapons/RangeWeapon.h"

// Sets default values
UInventoryComponent::UInventoryComponent()
{
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

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
			Cast<ARangeWeapon>(weapon)->FirePoint = FirePoint;
			weapon->DisableWeapon(true);
			Weapons.Add(weapon);
		}
		
	}
	Weapons[m_CurrentWeapon]->DisableWeapon(false);
}

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

void UInventoryComponent::FireCurrentWeapon()
{

	if (Weapons.IsValidIndex(m_CurrentWeapon))
	{
		
		Weapons[m_CurrentWeapon]->Fire();
		
	}
	
}

ABaseWeapon* UInventoryComponent::GetCurrentWeapon()
{

	if (Weapons.IsValidIndex(m_CurrentWeapon))
	{
		return Weapons[m_CurrentWeapon];
	}

	return  nullptr;
}
