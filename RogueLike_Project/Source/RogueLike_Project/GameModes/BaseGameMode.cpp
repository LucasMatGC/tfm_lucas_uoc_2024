// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

// Sets default values
ABaseGameMode::ABaseGameMode()
{
	
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetSeed();

	CurrentRooms = RandomStream.RandRange(MinRooms, MaxRooms);
	
	m_MapGenerator = GetWorld()->SpawnActor<AMapGenerator>(
			MapGeneratorType,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			FActorSpawnParameters());
	m_MapGenerator->Initialize(CurrentRooms, RandomStream);
	m_MapGenerator->GenerateRooms();

	m_Player = GetWorld()->GetFirstPlayerController();
	
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