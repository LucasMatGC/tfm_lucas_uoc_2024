﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/Character.h"
#include "RogueLike_Project/Objects/Weapons/MeleeWeapon.h"

// Sets default values
UHealthComponent::UHealthComponent()
{
	
}

// Called when the game starts or when spawned
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());
	character->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage);
	
}

// Called when the game ends or when despawned
void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	ACharacter* character = Cast<ACharacter>(GetOwner());
	character->OnTakeAnyDamage.RemoveDynamic(this, &UHealthComponent::OnTakeDamage);
	
	Super::EndPlay(EndPlayReason);
		
}

// Called when owner recieved any type of damage
void UHealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                    class AController* InstigatedBy, AActor* DamageCauser)
{
	TakeDamage(Damage, DamageCauser);
}


// Decreases life of actor by specified amount
void UHealthComponent::TakeDamage(float Damage, AActor* DamageCauser)
{

	if (Damage <= 0 || !bCanTakeDamage)
	{
		return;
	}

	float oldHealth = CurrentHealth;
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	
	UpdateHUD(oldHealth);
	bPlayedDamageAnimation = false;
	
	if (CurrentHealth <= 0)
	{
		ProcessDeath(DamageCauser);
	}
	
}

// Notifies the owner of the cause of the kill
void UHealthComponent::ProcessDeath(AActor* DamageCauser)
{

	if (AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(DamageCauser))
	{
		OnProcessDeath.Broadcast(true);
	}
	else
	{
		OnProcessDeath.Broadcast(false);
	}
	
}

void UHealthComponent::SetMaxHealth(float health)
{
	MaxHealth = health;
	CurrentHealth = MaxHealth;
}	

void UHealthComponent::SetHealth(float health)
{

	CurrentHealth = health;
	
	UpdateHUD(CurrentHealth);

}

float UHealthComponent::GetCurrentHealth()
{

	return CurrentHealth;
	
}

// Updates Max health. Used by upgrades
void UHealthComponent::UpgradeMaxHealth(float HealthUpgrade)
{

	float oldHealth = CurrentHealth;
	
	MaxHealth += HealthUpgrade;
	CurrentHealth += HealthUpgrade;
	
	UpdateHUD(oldHealth);
	
}

// Heal owner the specified amount
void UHealthComponent::Heal(float ConsumableHealth)
{

	float oldHealth = CurrentHealth;
	
	(CurrentHealth + ConsumableHealth) >= MaxHealth ? CurrentHealth = MaxHealth : CurrentHealth += ConsumableHealth;
	
	UpdateHUD(oldHealth);
	
}

// Updates the health UI widget
void UHealthComponent::UpdateHUD(float oldHealth)
{
	
	OnUpdateCurrentHealth.Broadcast(oldHealth, CurrentHealth, MaxHealth, CurrentHealth/MaxHealth);
	
}

void UHealthComponent::SetCanTakeDamage(bool CanTakeDamage)
{

	bCanTakeDamage = CanTakeDamage;
	
}
