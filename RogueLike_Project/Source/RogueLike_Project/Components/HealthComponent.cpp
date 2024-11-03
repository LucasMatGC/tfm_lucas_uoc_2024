// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "GameFramework/Character.h"
#include "RogueLike_Project/RogueLike_ProjectCharacter.h"
#include "RogueLike_Project/Items/Weapons/RangeWeapon.h"

// Sets default values
UHealthComponent::UHealthComponent()
{
	
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());
	character->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage);
	
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	ACharacter* character = Cast<ACharacter>(GetOwner());
	character->OnTakeAnyDamage.RemoveDynamic(this, &UHealthComponent::OnTakeDamage);
	
	Super::EndPlay(EndPlayReason);
		
}

void UHealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                    class AController* InstigatedBy, AActor* DamageCauser)
{
	TakeDamage(Damage);
}

void UHealthComponent::TakeDamage(float Damage)
{

	//TODO Return si daño negativo

	float oldHealth = CurrentHealth;
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	
	OnUpdatePlayerCurrentHealth.Broadcast(oldHealth, CurrentHealth, CurrentHealth/MaxHealth);
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "DAMAGE!!!!!!");

	if (CurrentHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "DEATH!!!!!!");
		ProcessDeath();
	}
	
}

void UHealthComponent::ProcessDeath()
{

	OnProcessDeath.Broadcast();
	
}
