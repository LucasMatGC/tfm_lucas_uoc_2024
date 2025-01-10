// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "RogueLike_Project/Objects/Weapons/RangeWeapon.h"
#include "RogueLike_Project/Utils/GameInstance/BaseGameInstance.h"

// Sets default values
AGameplayGameMode::AGameplayGameMode()
{
	
}

// Load the save data
void AGameplayGameMode::LoadLevelData()
{

	m_GameVariables =  m_GameInstance->GetGameVariables();

	CurrentGameTime = m_GameVariables.CurrentGameTime;
	
	
}

// Load next level based on current level
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

// Get a random integer from Game Instance
int AGameplayGameMode::RandomRangeInt(int Min, int Max)
{

	return m_GameInstance->RandomRangeInt(Min, Max);
	
}

// Get a random float from Game Instance
float AGameplayGameMode::RandomRangeFloat(float Min, float Max)
{
	
	return m_GameInstance->RandomRangeFloat(Min, Max);
	
}

// Called when the game starts or when spawned
void AGameplayGameMode::BeginPlay()
{
	Super::BeginPlay();
	

	if (m_GameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
	{

		FString levelName = GetWorld()->GetMapName();
		levelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

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

	ConsumableTable.LoadSynchronous();
	UpgradeTable.LoadSynchronous();
	
}

// Called every frame
void AGameplayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!m_IsPlayerPrepared)
	{
		PreparePlayer();
		m_IsPlayerPrepared = true;
	}
}

// Generate the seed of the level
void AGameplayGameMode::SetSeed()
{

	m_GameInstance->SetSeed();
	
}

// Prepare the player spawning it in the proper point and with the save data
void AGameplayGameMode::PreparePlayer()
{

	TArray<USceneComponent*> spawnPoint;
	
	MapGenerator->SpawnRoom->SpawnPointFolder->GetChildrenComponents(false, spawnPoint);

	if (spawnPoint.Num() > 0)
	{

		if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(Player->GetCharacter()))
		{
			
			Character->SetActorLocation(spawnPoint[0]->GetComponentLocation());

			// If not the first level, give player saved upgrades to corresponding weapons
			if (CurrentLevelConfiguration.CurrentLevel > 1)
			{
					
				
				Character->HealthComponent->SetHealth(m_GameVariables.PlayerHealth);
				for (FUpgradeStruct Upgrade : m_GameVariables.ListOfCommonUpgrades)
				{

					Character->InventoryComponent->AddUpgrade(Upgrade, -1);
					
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

// Load result level with game over message
void AGameplayGameMode::GameOver()
{
			
	m_GameInstance->SetGameVariables(Player, 30.0f, true, false);
	UGameplayStatics::OpenLevel(GetWorld(), "05_Results");
	
}

// Check if an item should spawn based on the enemy spawn rate
void AGameplayGameMode::CheckItemSpawn(float RandomizedItemSpawnRate, FTransform SpawnTransform, bool isMeleeDamage)
{

	// If spawn rate doesn't pass the consumable threshold, return
	if (RandomizedItemSpawnRate < ConsumableSpawnThreshold)
	{
		return;
	}

	// If spawn rate doesn't pass the upgrade threshold, spawn consumable
	if (RandomizedItemSpawnRate < UpgradeSpawnThreshold)
	{

		ConsumableTable.LoadSynchronous();
		
		TArray<FName> RowNames = ConsumableTable->GetRowNames();
		
		FActorSpawnParameters spawnInfo;

		FItemDataRow* itemData;
		
		if (isMeleeDamage)
		{
			itemData = ConsumableTable->FindRow<FItemDataRow>( RowNames[1], nullptr, true );
		}
		else
		{
			itemData = ConsumableTable->FindRow<FItemDataRow>( RowNames[0], nullptr, true );
		}
		
		TObjectPtr<ABaseItem> newItem = GetWorld()->SpawnActorDeferred<ABaseItem>(
					itemData->ItemBP,
					SpawnTransform,
					nullptr,
					nullptr);
		
		newItem->ItemName = itemData->DisplayName;
		newItem->FinishSpawning(SpawnTransform, false, nullptr);
	}
	// If spawn pass upgrade threshold, spawn it
	else
	{
		
		UpgradeTable.LoadSynchronous();
		
		TArray<FName> RowNames = UpgradeTable->GetRowNames();

		FActorSpawnParameters spawnInfo;

		FItemDataRow* itemData = UpgradeTable->FindRow<FItemDataRow>( RowNames[RandomRangeInt(0, RowNames.Num() - 1)], nullptr, true ); 
		
		TObjectPtr<ABaseItem> newItem = GetWorld()->SpawnActorDeferred<ABaseItem>(
					itemData->ItemBP,
					SpawnTransform,
					nullptr,
					nullptr);
		
		newItem->ItemName = itemData->DisplayName;
		newItem->FinishSpawning(SpawnTransform, false, nullptr);
	}
	
}