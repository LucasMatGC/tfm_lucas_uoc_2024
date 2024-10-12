// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRoom.h"

#include "Chaos/DebugDrawQueue.h"

// Sets default values
ABaseRoom::ABaseRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	ExitsFolder = CreateDefaultSubobject<USceneComponent>(TEXT("ExitsFolder"));
	GeometryFolder = CreateDefaultSubobject<USceneComponent>(TEXT("GeometryFolder"));
	OverlapFolder = CreateDefaultSubobject<USceneComponent>(TEXT("OverlapFolder"));
	Direction = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"));

	RootScene->SetupAttachment(RootComponent);
	GeometryFolder->SetupAttachment(RootScene);
	ExitsFolder->SetupAttachment(RootScene);
	OverlapFolder->SetupAttachment(RootScene);
	Direction->SetupAttachment(RootScene);

}

// Called when the game starts or when spawned
void ABaseRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


