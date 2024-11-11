// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDoor.h"

// Sets default values
ABaseDoor::ABaseDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootScene);
}

void ABaseDoor::OpenDoor()
{

	if (!m_bIsLocked)
	{
		SetActorHiddenInGame(true);

		SetActorEnableCollision(false);
	}
	
}

void ABaseDoor::CloseDoor()
{
	if (!m_bIsLocked)
	{
		SetActorHiddenInGame(false);

		SetActorEnableCollision(true);
	}
}

bool ABaseDoor::IsLocked()
{
	return m_bIsLocked;
}

void ABaseDoor::SetLocked(bool IsLocked)
{

	m_bIsLocked = IsLocked;

	SetActorHiddenInGame(!IsLocked);

	SetActorEnableCollision(IsLocked);
	
}


