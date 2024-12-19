// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

// Sets default values
ABaseGameMode::ABaseGameMode()
{
	
}

void ABaseGameMode::PauseGame()
{

	m_IsGamePaused = !m_IsGamePaused;

	//TODO: llamar al widget de pausa para mostrar el menu. Tambien se llama a este metodo al despausar, tener en cuenta
	
}

int ABaseGameMode::RandomRangeInt(int Min, int Max)
{

	return RandomStream.RandRange(Min, Max);
	
}

float ABaseGameMode::RandomRangeFloat(float Min, float Max)
{
	return RandomStream.RandRange(Min, Max);
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
	m_MapGenerator->Initialize(CurrentRooms, this);
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