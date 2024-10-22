// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "GameFramework/Character.h"

// Sets default values
UInventoryComponent::UInventoryComponent()
{
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	for (TSubclassOf<ABaseWeapon> weaponType : WeaponTypes)
	{
		FActorSpawnParameters spawnInfo;
		FVector spawnLocation = FVector::ZeroVector;
		FRotator spawnRotation = FRotator::ZeroRotator;
		
		ABaseWeapon* weapon = GetWorld()->SpawnActor<ABaseWeapon>(weaponType, spawnLocation, spawnRotation, spawnInfo);
		if (ACharacter* character = Cast<ACharacter>(GetOwner()))
		{
			weapon->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
			Weapons.Add(weapon);
		}
		
	}
}

void UInventoryComponent::ChangeWeapon(bool MoveForward)
{
	if (Weapons.Num() > 1)
	{
		
		if (MoveForward)
		{
			m_CurrentWeapon == Weapons.Num() - 1 ? m_CurrentWeapon = 0 : m_CurrentWeapon++;
		}
		else
		{
			m_CurrentWeapon == 0 ? m_CurrentWeapon = Weapons.Num() - 1 : m_CurrentWeapon--;
		}
	
	}
}

void UInventoryComponent::FireCurrentWeapon()
{
	Weapons[m_CurrentWeapon]->Fire();
}
