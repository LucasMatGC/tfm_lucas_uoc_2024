// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDoor.h"

// Sets default values
ABaseDoor::ABaseDoor()
{
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootScene);
}

// Open all unlocked doors
void ABaseDoor::OpenDoor()
{

	if (!m_bIsLocked)
	{
		SetActorHiddenInGame(true);

		SetActorEnableCollision(false);
	}
	
}

// Close all doors without locking them
void ABaseDoor::CloseDoor()
{
	if (!m_bIsLocked)
	{
		SetActorHiddenInGame(false);

		SetActorEnableCollision(true);
	}
}

// Check if door is locked
bool ABaseDoor::IsLocked()
{
	return m_bIsLocked;
}

// Set the door lock. If Locked, door won't open without unlocking it
void ABaseDoor::SetLocked(bool IsLocked)
{

	m_bIsLocked = IsLocked;

	SetActorHiddenInGame(!IsLocked);

	SetActorEnableCollision(IsLocked);
	
}


