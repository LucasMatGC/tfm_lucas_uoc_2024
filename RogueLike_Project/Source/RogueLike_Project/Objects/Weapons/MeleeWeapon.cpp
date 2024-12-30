// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "RogueLike_Project/RogueLike_ProjectCharacter.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{

	
	
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMeleeWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsAttacking)
	{

		CurrentFireRate -= DeltaSeconds;

		if (CurrentFireRate < 0)
		{
			bIsAttacking = false;
			bCanDoComboAttack = false;
			CurrentCombo = 0;
			HideAttackCollision();
		}

		if (CurrentComboRate > 0 )
		{	
			CurrentComboRate -= DeltaSeconds;
		}
		
		if (CurrentComboRate < 0)
		{
			
			HideAttackCollision();
			
			if (CurrentCombo < MaxCombo)
			{
				bCanDoComboAttack = true;
			}
			
		}
		
	}

}

void AMeleeWeapon::Fire()
{
	Super::Fire();

	if (AttackMesh == nullptr)
	{
		return;
	}
	
	if (!bIsAttacking || bCanDoComboAttack)
	{
	
		bIsAttacking = true;
		CurrentFireRate = MaxFireRate;
		CurrentComboRate = MaxComboRate;
		bCanDoComboAttack = false;
		CurrentCombo++;
		
		if (ARogueLike_ProjectCharacter* Player = Cast<ARogueLike_ProjectCharacter>(GetAttachParentActor()))
		{
			
			OnWeaponFire.Broadcast(true, CurrentCombo);
			
			Player->UseMeleeCollider(Range, true);
			
		}
		
	}
	
}

void AMeleeWeapon::HideAttackCollision()
{
		
	if (ARogueLike_ProjectCharacter* Player = Cast<ARogueLike_ProjectCharacter>(GetAttachParentActor()))
	{

		Player->UseMeleeCollider(Range, false);
			
	}
	
}

float AMeleeWeapon::CalculateDamage() const
{

	return (BaseDamage + AddedDamage) * DamageMultiplier;
	
}

void AMeleeWeapon::ApplyDamage(AActor* OtherActor)
{

	UGameplayStatics::ApplyDamage(OtherActor, CalculateDamage(), GetWorld()->GetFirstPlayerController(), this, UDamageType::StaticClass());
	
}
