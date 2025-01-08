// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"

#include "Chaos/ChaosPerfTest.h"
#include "RogueLike_Project/Objects/Weapons/RangeWeapon.h"

// Sets default values
UBaseGameInstance::UBaseGameInstance()
{
	
}

void UBaseGameInstance::SetGameVariables(APlayerController* CurrentPlayer, float CurrentGameTime,
                                         bool IsGameFinished, bool IsGameWon)
{

	SavedGameVariables = FGameVariables();

	SavedGameVariables.CurrentGameTime = CurrentGameTime;
	SavedGameVariables.bIsGameFinished = IsGameFinished;
	SavedGameVariables.bIsGameWon = IsGameWon;

	if (ARogueLike_ProjectCharacter* Player = Cast<ARogueLike_ProjectCharacter>(CurrentPlayer->GetCharacter()))
	{
		
		SavedGameVariables.PlayerHealth = Player->HealthComponent->GetCurrentHealth();
		SavedGameVariables.ListOfCommonUpgrades = Player->InventoryComponent->CommonUpgrades;

		for (int i = 0 ; i < Player->InventoryComponent->Weapons.Num() ; i++)
		{

			FWeaponVariables WeaponData = FWeaponVariables();

			if (ARangeWeapon* RangedWeapon = Cast<ARangeWeapon>(Player->InventoryComponent->Weapons[i]))
			{
				WeaponData.WeaponsAmmo = RangedWeapon->CurrentAmmo;
			}
			else
			{
				WeaponData.WeaponsAmmo = 0;
			}
			
			WeaponData.ListOfUpgrades = Player->InventoryComponent->Weapons[i]->Upgrades;

			SavedGameVariables.WeaponInfo.Add(i, WeaponData);
			
		}

	}
	
}

FGameVariables UBaseGameInstance::GetGameVariables()
{

	return SavedGameVariables;
	
}

int UBaseGameInstance::RandomRangeInt(int Min, int Max)
{

	return RandomStream.RandRange(Min, Max);
	
}

float UBaseGameInstance::RandomRangeFloat(float Min, float Max)
{
	return RandomStream.FRandRange(Min, Max);
}

void UBaseGameInstance::SetSeed()
{

	if (PredefinedSeed == -1)
	{

		RandomStream.GenerateNewSeed();
		PredefinedSeed = RandomStream.GetCurrentSeed();
		
	}
	else
	{
		RandomStream.Initialize(PredefinedSeed);
	}

	SavedGameVariables.CurrentGameTime = 0.0f;
	
}
