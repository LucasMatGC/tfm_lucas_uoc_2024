// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"
#include "RogueLike_Project/GameModes/GameplayGameMode.h"
#include "Components/BoxComponent.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootScene->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (MaxRoom <= 0)
		MaxRoom = 8;

	
}

//Initialize variables for the MapGenerator
void AMapGenerator::Initialize(int maxRooms, AGameplayGameMode* GameMode)
{

	m_GameMode = GameMode;
	MaxRoom = maxRooms;
	
}

// Generate the rooms for the level
void AMapGenerator::GenerateRooms()
{
	
	if (MaxRoom <= 0 || MaxRoom >= 20)
	{

		return;
		
	}

	FVector location(0.0f, 0.0f, 0.0f);
	FRotator rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawnInfo;
	
	m_LatestRoom = GetWorld()->SpawnActor<ABaseRoom>(StartRoomType, location, rotation, spawnInfo);
	SpawnRoom = m_LatestRoom;
	SpawnRoom->PrepareRoom(m_GameMode);
	AddExits();

	if (ExitsLists.Num() > 0 && CurrentRooms < MaxRoom)
	{
		SpawnNextRoom();
	}
	
}

// Spawn a basic room on an available exit
void AMapGenerator::SpawnNextRoom()
{

	//Get Random Room Type and random exit to spawn
	TSubclassOf<ABaseRoom> roomClass = GetRandomRoomType();
	USceneComponent* exit = GetRandomExit();
	FActorSpawnParameters spawnInfo;

	if (roomClass == nullptr || exit == nullptr)
		return;
	
	m_LatestRoom = GetWorld()->SpawnActor<ABaseRoom>(
		roomClass,
		exit->GetComponentTransform().GetLocation(),
		exit->GetComponentRotation(),
		spawnInfo);
	
	// Check if is overlaping with existing room. If true, destroy room and try with another exit
	if (IsOverlaping())
	{

		ExitsLists.Remove(exit);
		if (ABaseRoom* room = Cast<ABaseRoom>(exit->GetAttachParentActor()))
		{
			room->SpawnDoor(exit->GetComponentLocation(), exit->GetComponentRotation(), true);
		}
		m_LatestRoom->Destroy();
		SpawnNextRoom();
		
	}
	// Creates a door for the room and calls the prepare room function.
	else
	{

		m_LatestRoom->PrepareRoom(m_GameMode);
		if (ABaseRoom* room = Cast<ABaseRoom>(exit->GetAttachParentActor()))
		{
			room->SpawnDoor(exit->GetComponentLocation(), exit->GetComponentRotation(), false);
		}
		CurrentRooms++;
		AddExits();
		// If more rooms are needed, spawn next room
		if (CurrentRooms < MaxRoom && ExitsLists.Num() > 0)
		{

			ExitsLists.Remove(exit);
			SpawnNextRoom();
			
		}
		// Else spawn boss and item room and close remaining exits
		else
		{

			ExitsLists.Remove(exit);
			SpawnBossRoom();
			SpawnItemRoom();
			CloseRemainingExits();
			
		}
		
	}
	
}

// Add the exits from the last created room to the list.
void AMapGenerator::AddExits()
{

	TArray<USceneComponent*> exits;
	m_LatestRoom->ExitsFolder->GetChildrenComponents(false, exits);
	
	ExitsLists.Append(exits);
}

// Check if last created room overlaps with the already created ones
bool AMapGenerator::IsOverlaping()
{

	UBoxComponent* collider = Cast<UBoxComponent>(m_LatestRoom->OverlapFolder->GetChildComponent(0));

	TArray<UPrimitiveComponent*> overlappingComponents = {};
	collider->GetOverlappingComponents(overlappingComponents);
		
	return overlappingComponents.Num() != 0;
	
}

// Spawn a boss room on an available exit
void AMapGenerator::SpawnBossRoom()
{

	bool bIsSpawnValid = false;

	//Until the spawn is valid, try to spawn it in the remaining exits
	while (!bIsSpawnValid)
	{
		
		USceneComponent* exit = GetRandomExit();
		FActorSpawnParameters spawnInfo;

		if (BossRoomType == nullptr || exit == nullptr)
		{
			UE_LOG(LogSpawn, Error, TEXT("Boss Room not defined"));
			return;
			
		}
		
		m_LatestRoom = GetWorld()->SpawnActor<ABaseRoom>(
			BossRoomType,
			exit->GetComponentTransform().GetLocation(),
			exit->GetComponentRotation(),
			spawnInfo);

		
		// Check if is overlaping with existing room. If true, destroy room and try with another exit
		if (IsOverlaping())
		{

			ExitsLists.Remove(exit);
			m_LatestRoom->Destroy();
			
		}
		// Creates a door for the room and calls the prepare room function.
		else
		{
			if (ABaseRoom* room = Cast<ABaseRoom>(exit->GetAttachParentActor()))
			{
				room->SpawnDoor(exit->GetComponentLocation(), exit->GetComponentRotation(), false);
			}
			ExitsLists.Remove(exit);
			m_LatestRoom->PrepareRoom(m_GameMode);
			BossRoom = m_LatestRoom;
			RoomList.Add(BossRoom);
			bIsSpawnValid = true;
		}
		
	}
}

// Spawn an item room on an available exit
void AMapGenerator::SpawnItemRoom()
{

	bool bIsSpawnValid = false;

	//Until the spawn is valid, try to spawn it in the remaining exits
	while (!bIsSpawnValid)
	{
		
		USceneComponent* exit = GetRandomExit();
		FActorSpawnParameters spawnInfo;

		//If no Item room type is defined or no exits are available,
		if (ItemRoomType == nullptr || exit == nullptr)
		{
			UE_LOG(LogSpawn, Error, TEXT("Item Room not defined"));
			return;
			
		}
		
		m_LatestRoom = GetWorld()->SpawnActor<ABaseRoom>(
			ItemRoomType,
			exit->GetComponentTransform().GetLocation(),
			exit->GetComponentRotation(),
			spawnInfo);

		// Check if is overlaping with existing room. If true, destroy room and try with another exit
		if (IsOverlaping())
		{

			ExitsLists.Remove(exit);
			m_LatestRoom->Destroy();
			
		}
		// Creates a door for the room and calls the prepare room function.
		else
		{
			if (ABaseRoom* room = Cast<ABaseRoom>(exit->GetAttachParentActor()))
			{
				room->SpawnDoor(exit->GetComponentLocation(), exit->GetComponentRotation(), false);
			}
			ExitsLists.Remove(exit);
			m_LatestRoom->PrepareRoom(m_GameMode);
			ItemRoom = m_LatestRoom;
			RoomList.Add(ItemRoom);
			bIsSpawnValid = true;
		}
		
	}
}

// Closes the remaining exits and locks doors
void AMapGenerator::CloseRemainingExits()
{
	for (USceneComponent* exit : ExitsLists)
	{

		if (ABaseRoom* room = Cast<ABaseRoom>(exit->GetAttachParentActor()))
		{
			
			room->SpawnDoor(exit->GetComponentLocation(), exit->GetComponentRotation(), true);
		}
		
	}
	
	
}

// Return a random room type to spawn
TSubclassOf<ABaseRoom> AMapGenerator::GetRandomRoomType()
{

	//If no room types listed, return nullptr
	if (RoomTypeList.Num() == 0 )
	{

		return nullptr;
		
	}
	
	return RoomTypeList[m_GameMode->RandomRangeInt(0, RoomTypeList.Num() - 1)];
	
}

// Return a random exit from all available
USceneComponent* AMapGenerator::GetRandomExit()
{

	//If no room types listed, return nullptr
	if (ExitsLists.Num() == 0 )
	{

		return nullptr;
		
	}

	return ExitsLists[m_GameMode->RandomRangeInt(0, ExitsLists.Num() - 1)];
	
}

