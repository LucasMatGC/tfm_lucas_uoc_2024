// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"
#include "RogueLike_Project/GameModes/BaseGameMode.h"
#include "Components/BoxComponent.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
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

void AMapGenerator::Initialize(int maxRooms, ABaseGameMode* GameMode)
{

	m_GameMode = GameMode;
	MaxRoom = maxRooms;
	
}

// Called every frame
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
	AddExits();

	if (ExitsLists.Num() > 0 && CurrentRooms < MaxRoom)
	{
		SpawnNextRoom();
	}
	
}

// Called every frame
void AMapGenerator::SpawnNextRoom()
{

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

	
	if (IsOverlaping())
	{

		ExitsLists.Remove(exit);
		m_LatestRoom->Destroy();
		SpawnNextRoom();
		
	}
	//TODO: Preparar la generación interna de elementos/enemigos de cada sala en este punto para asegurar una aleatoriedad
	// continua
	else
	{

		m_LatestRoom->PrepareRoom(m_GameMode);
		CurrentRooms++;
		AddExits();
		if (CurrentRooms < MaxRoom && ExitsLists.Num() > 0)
		{

			if (ABaseRoom* room = Cast<ABaseRoom>(exit->GetAttachParentActor()))
			{
				room->SpawnDoor(exit->GetComponentLocation(), exit->GetComponentRotation(), false);
			}
			ExitsLists.Remove(exit);
			SpawnNextRoom();
			
		}
		else
		{

			ExitsLists.Remove(exit);
			SpawnBossRoom();
			SpawnItemRoom();
			CloseRemainingExits();
			
		}
		
	}
	
}

// Called every frame
void AMapGenerator::AddExits()
{

	TArray<USceneComponent*> exits = {};
	exits = m_LatestRoom->ExitsFolder->GetAttachChildren();
	
	ExitsLists.Append(exits);
}

// Called every frame
bool AMapGenerator::IsOverlaping()
{

	UBoxComponent* collider = Cast<UBoxComponent>(m_LatestRoom->OverlapFolder->GetChildComponent(0));
	//Check if collides with any other generated room

	TArray<UPrimitiveComponent*> overlappingComponents = {};
	collider->GetOverlappingComponents(overlappingComponents);
		
	return overlappingComponents.Num() != 0;
	
}

void AMapGenerator::SpawnBossRoom()
{

	bool bIsSpawnValid = false;

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

		ExitsLists.Remove(exit);
		
		if (IsOverlaping())
		{

			m_LatestRoom->Destroy();
			
		}
		else
		{
			BossRoom = m_LatestRoom;
			RoomList.Add(BossRoom);
			bIsSpawnValid = true;
		}
		
	}
}

void AMapGenerator::SpawnItemRoom()
{

	bool bIsSpawnValid = false;

	while (!bIsSpawnValid)
	{
		
		USceneComponent* exit = GetRandomExit();
		FActorSpawnParameters spawnInfo;

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

		ExitsLists.Remove(exit);
		
		if (IsOverlaping())
		{

			m_LatestRoom->Destroy();
			
		}
		else
		{
			ItemRoom = m_LatestRoom;
			RoomList.Add(ItemRoom);
			bIsSpawnValid = true;
		}
		
	}
}

// Called every frame
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

// Called every frame
TSubclassOf<ABaseRoom> AMapGenerator::GetRandomRoomType()
{

	//If no room types listed, return nullptr
	if (RoomTypeList.Num() == 0 )
	{

		return nullptr;
		
	}
	
	return RoomTypeList[m_GameMode->RandomRangeInt(0, RoomList.Num() - 1)];
	
}

// Called every frame
USceneComponent* AMapGenerator::GetRandomExit()
{

	//If no room types listed, return nullptr
	if (ExitsLists.Num() == 0 )
	{

		return nullptr;
		
	}

		return ExitsLists[m_GameMode->RandomRangeInt(0, ExitsLists.Num() - 1)];
	
}

