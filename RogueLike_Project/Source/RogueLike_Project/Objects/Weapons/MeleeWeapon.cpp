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

	if(LifeSteal > 0.0)
	{

		if (ARogueLike_ProjectCharacter* Player = Cast<ARogueLike_ProjectCharacter>(GetAttachParentActor()))
		{
			float HealAmmount = FMathf::Floor(LifeSteal * CalculateDamage());
		
			Player->HealPlayer(HealAmmount);	
		}
	}
	
}

void AMeleeWeapon::AddUpgrade(FUpgradeStruct newUpgrade, bool bIsCommonUpgrade)
{
	Super::AddUpgrade(newUpgrade, bIsCommonUpgrade);

	if (newUpgrade.UpgradeType == EUpgradeType::BaseVariablesUpgrade || newUpgrade.UpgradeType != EUpgradeType::CustomizedUpgrade)
	{
		
		AddedDamage += FMath::Max(newUpgrade.AddedDamage, 0.0f);
		DamageMultiplier += FMath::Max(newUpgrade.AddedDamageMultiplier, 0.1f);
		Range += FMath::Max(newUpgrade.ExtraScale, 0.1f);
		MaxFireRate -= FMath::Max(newUpgrade.ReducedFireRate, 0.1f);
		MaxCombo += FMath::Max(newUpgrade.AddedCombo, 1);

	}

	if (newUpgrade.UpgradeType == EUpgradeType::ModifierUpgrade || newUpgrade.UpgradeType != EUpgradeType::CustomizedUpgrade)
	{
		
		LifeSteal += FMath::Max(newUpgrade.LifeSteal, 0.0f);

	}
}
