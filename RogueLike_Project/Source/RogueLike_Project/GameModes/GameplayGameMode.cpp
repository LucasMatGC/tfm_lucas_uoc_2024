// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "RogueLike_Project/Objects/Weapons/RangeWeapon.h"
#include "RogueLike_Project/Utils/GameInstance/BaseGameInstance.h"

// Sets default values
AGameplayGameMode::AGameplayGameMode()
{
	
}

void AGameplayGameMode::LoadLevelData()
{

	m_GameVariables =  m_GameInstance->GetGameVariables();

	CurrentGameTime = m_GameVariables.CurrentGameTime;
	
	
}

void AGameplayGameMode::LoadNextLevel()
{

	switch (CurrentLevelConfiguration.CurrentLevel)
	{
	
		case 1:
			
			m_GameInstance->SetGameVariables(Player, 10.0f, false, false);
			UGameplayStatics::OpenLevel(GetWorld(), "03_SecondLevel");
			break;
		
		case 2:
		
			m_GameInstance->SetGameVariables(Player, 20.0f, false, false);
			UGameplayStatics::OpenLevel(GetWorld(), "04_ThirdLevel");
			break;
			
		case 3:
			
			m_GameInstance->SetGameVariables(Player, 30.0f, true, true);
			UGameplayStatics::OpenLevel(GetWorld(), "05_Results");
			break;
		
	}
	
}

int AGameplayGameMode::RandomRangeInt(int Min, int Max)
{

	return m_GameInstance->RandomRangeInt(Min, Max);
	
}

float AGameplayGameMode::RandomRangeFloat(float Min, float Max)
{
	
	return m_GameInstance->RandomRangeFloat(Min, Max);
	
}

void AGameplayGameMode::BeginPlay()
{
	Super::BeginPlay();
	

	if (m_GameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
	{

		FString levelName = GetWorld()->GetMapName();
		levelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, levelName);

		if (FLevelVariables* currentLevelConfiguration = LevelConfigurationMap.Find(FName(levelName)))
		{

			CurrentLevelConfiguration = *currentLevelConfiguration;

			if (CurrentLevelConfiguration.CurrentLevel == 1)
			{
				
				SetSeed();
		
			}
			else
			{
				
				LoadLevelData();
				
			}

			CurrentRooms = RandomRangeInt(CurrentLevelConfiguration.MinRooms, CurrentLevelConfiguration.MaxRooms);
		
			MapGenerator = GetWorld()->SpawnActor<AMapGenerator>(
					MapGeneratorType,
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					FActorSpawnParameters());
			MapGenerator->Initialize(CurrentRooms, this);
			MapGenerator->GenerateRooms();
		
			Player = GetWorld()->GetFirstPlayerController();
		}
		
	}

	
	
}

void AGameplayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!m_IsPlayerPrepared)
	{
		PreparePlayer();
		m_IsPlayerPrepared = true;
	}
}

void AGameplayGameMode::SetSeed()
{

	m_GameInstance->SetSeed();
	
}

void AGameplayGameMode::PreparePlayer()
{

	TArray<USceneComponent*> spawnPoint;
	
	MapGenerator->SpawnRoom->SpawnPointFolder->GetChildrenComponents(false, spawnPoint);

	if (spawnPoint.Num() > 0)
	{

		if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(Player->GetCharacter()))
		{
			
			Character->SetActorLocation(spawnPoint[0]->GetComponentLocation());

			if (CurrentLevelConfiguration.CurrentLevel > 1)
			{
					
				
				Character->HealthComponent->SetHealth(m_GameVariables.PlayerHealth);
				for (FUpgradeStruct Upgrade : m_GameVariables.ListOfCommonUpgrades)
				{

					Character->InventoryComponent->AddUpgrade(Upgrade, 0);
					
				}

				for (int i = 0; i < Character->InventoryComponent->Weapons.Num(); i++)
				{

					TArray<FUpgradeStruct> Upgrades = m_GameVariables.WeaponInfo[i].ListOfUpgrades;
					for (FUpgradeStruct Upgrade : Upgrades)
					{

						Character->InventoryComponent->AddUpgrade(Upgrade, i);	
						
					}

					if (ARangeWeapon* RangeWeapon = Cast<ARangeWeapon>(Character->InventoryComponent->Weapons[i]))
					{

						RangeWeapon->CurrentAmmo = m_GameVariables.WeaponInfo[i].WeaponsAmmo;
						
					}
					
				}
				
			}
			
		}
		
	}
	
}

void AGameplayGameMode::GameOver()
{
			
	m_GameInstance->SetGameVariables(Player, 30.0f, true, false);
	UGameplayStatics::OpenLevel(GetWorld(), "05_Results");
	
}
