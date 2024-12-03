// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/Character.h"

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

	if (Damage <= 0)
	{
		return;
	}

	float oldHealth = CurrentHealth;
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	
	UpdateHUD(oldHealth);
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("DAMAGE Taken: %f, oldHealth: %f, currentHealth: %f"), Damage, oldHealth, CurrentHealth));

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

void UHealthComponent::SetMaxHealth(float health)
{

	MaxHealth = health;
	CurrentHealth = MaxHealth;

}

void UHealthComponent::UpgradeMaxHealth(float HealthUpgrade)
{

	float oldHealth = CurrentHealth;
	
	MaxHealth += HealthUpgrade;
	CurrentHealth += HealthUpgrade;
	
	UpdateHUD(oldHealth);
	
}

void UHealthComponent::Heal(float ConsumableHealth)
{

	float oldHealth = CurrentHealth;
	
	(CurrentHealth + ConsumableHealth) >= MaxHealth ? CurrentHealth = MaxHealth : CurrentHealth += ConsumableHealth;
	
	UpdateHUD(oldHealth);
	
}

void UHealthComponent::UpdateHUD(float oldHealth)
{
	
	OnUpdateCurrentHealth.Broadcast(oldHealth, CurrentHealth, CurrentHealth/MaxHealth);
	
}
