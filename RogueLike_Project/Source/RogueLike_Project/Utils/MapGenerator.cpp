// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"

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

	ExitsLists.Remove(exit);
	
	if (IsOverlaping())
	{

		ExitsLists.Remove(exit);
		m_LatestRoom->Destroy();
		SpawnNextRoom();
		
	}
	else
	{

		CurrentRooms++;
		AddExits();
		if (CurrentRooms < MaxRoom && ExitsLists.Num() > 0)
		{

			SpawnNextRoom();
			
		}
		else
		{

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

// Called every frame
void AMapGenerator::CloseRemainingExits()
{
	for (USceneComponent* exit : ExitsLists)
	{

		FActorSpawnParameters spawnInfo;
		
		GetWorld()->SpawnActor<AActor>(
			DoorBP,
			exit->GetComponentTransform().GetLocation(),
			exit->GetComponentRotation(),
			spawnInfo);
		
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
	
	return RoomTypeList[rand() % RoomTypeList.Num()];
	
}

// Called every frame
USceneComponent* AMapGenerator::GetRandomExit()
{

	//If no room types listed, return nullptr
	if (ExitsLists.Num() == 0 )
	{

		return nullptr;
		
	}
	
	return ExitsLists[rand() % ExitsLists.Num()];
	
}

