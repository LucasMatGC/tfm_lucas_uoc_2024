// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseGameMode::ABaseGameMode()
{
	
}

int ABaseGameMode::RandomRangeInt(int Min, int Max)
{

	return RandomStream.RandRange(Min, Max);
	
}

float ABaseGameMode::RandomRangeFloat(float Min, float Max)
{
	return RandomStream.RandRange(Min, Max);
}

void ABaseGameMode::LoadNextLevel()
{

	switch (CurrentLevelConfiguration.CurrentLevel)
	{
	
		case 1:
			
			UGameplayStatics::OpenLevel(GetWorld(), "03_SecondLevel");
			break;
		
		case 2:
		
			UGameplayStatics::OpenLevel(GetWorld(), "04_ThirdLevel");
			break;
			
		case 3:
			
			UGameplayStatics::OpenLevel(GetWorld(), "05_Results");
			break;
		
	}
	
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	

	FString levelName = GetWorld()->GetMapName();
	levelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, levelName);

	if (FLevelVariables* currentLevelConfiguration = LevelConfigurationMap.Find(FName(levelName)))
	{

		CurrentLevelConfiguration = *currentLevelConfiguration;

		//if (CurrentLevelConfiguration.CurrentLevel == 1)
		//{
			
			SetSeed();
		
		//}

		CurrentRooms = RandomStream.RandRange(CurrentLevelConfiguration.MinRooms, CurrentLevelConfiguration.MaxRooms);
		
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

void ABaseGameMode::SetSeed()
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
	
}