// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRoom.h"

#include "AIController.h"
#include "BaseDoor.h"
#include "BrainComponent.h"
#include "Components/BoxComponent.h"
#include "RogueLike_Project/RogueLike_ProjectCharacter.h"
#include "RogueLike_Project/GameModes/BaseGameMode.h"

// Sets default values
ABaseRoom::ABaseRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	ExitsFolder = CreateDefaultSubobject<USceneComponent>(TEXT("ExitsFolder"));
	GeometryFolder = CreateDefaultSubobject<USceneComponent>(TEXT("GeometryFolder"));
	OverlapFolder = CreateDefaultSubobject<USceneComponent>(TEXT("OverlapFolder"));
	SpawnPointFolder = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPointFolder"));
	ItemsFolder = CreateDefaultSubobject<USceneComponent>(TEXT("ItemsFolder"));
	TriggersFolder = CreateDefaultSubobject<USceneComponent>(TEXT("TriggersFolder"));
	Direction = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"));

	SetRootComponent(RootScene);
	GeometryFolder->SetupAttachment(RootScene);
	ExitsFolder->SetupAttachment(RootScene);
	OverlapFolder->SetupAttachment(RootScene);
	SpawnPointFolder->SetupAttachment(RootScene);
	ItemsFolder->SetupAttachment(RootScene);
	TriggersFolder->SetupAttachment(RootScene);
	Direction->SetupAttachment(RootScene);

}

void ABaseRoom::PrepareRoom(ABaseGameMode* GameMode)
{

	m_GameMode = GameMode;
	
	int numberOfEnemiesToSpawn = 0;
	int numberOfItemsToSpawn = 0;

	switch (Functionality)
	{
	case ERoomFunctionality::RandomEnemies:
		SpawnEnemies(SpawnPoints.Num());
		break;
			
	case ERoomFunctionality::RandomItems:
		SpawnItems(SpawnPoints.Num());
		break;

			
	case ERoomFunctionality::FullyRandom:
		numberOfEnemiesToSpawn = m_GameMode->RandomRangeInt(0, SpawnPoints.Num());
		numberOfItemsToSpawn = SpawnPoints.Num() - numberOfEnemiesToSpawn;
		SpawnEnemies(numberOfEnemiesToSpawn);
		SpawnItems(numberOfItemsToSpawn);
		break;
			
		// Customized
	default:
		SpawnCustom();
		break;
				
	}
	
}

void ABaseRoom::SpawnDoor(FVector DoorLocation, FRotator DoorRotation, bool IsDoorLocked)
{

	FActorSpawnParameters spawnInfo;
	
	ABaseDoor* door = GetWorld()->SpawnActor<ABaseDoor>(
			DoorBP,
			DoorLocation,
			DoorRotation,
			spawnInfo);

	Doors.Add(door);
	door->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "Door");
	door->SetLocked(IsDoorLocked);
	
}

// Called when the game starts or when spawned
void ABaseRoom::BeginPlay()
{
	
	Super::BeginPlay();

	TArray<USceneComponent*> TriggerBoxes;
	TriggersFolder->GetChildrenComponents(false, TriggerBoxes);
	
	for (USceneComponent* trigger : TriggerBoxes)
	{

		if (UBoxComponent* boxColliderTrigger = Cast<UBoxComponent>(trigger))
		{
			
			boxColliderTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABaseRoom::PlayerEnters);
			
		}
		
	}

	FActorSpawnParameters spawnInfo;

	//Spawn a door on the entry
	ABaseDoor* door = GetWorld()->SpawnActor<ABaseDoor>(
		DoorBP,
		this->GetTransform().GetLocation(),
		this->GetActorRotation(),
		spawnInfo);

	Doors.Add(door);
	door->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "Door");

	door->SetLocked(false);

	SpawnPointFolder->GetChildrenComponents(false, SpawnPoints);

	EnemyTable.LoadSynchronous();
	ItemTable.LoadSynchronous();
	
}

// Called every frame
void ABaseRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseRoom::PlayerEnters(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SpawnedEnemies.Num() > 0)
	{
		
		if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(OtherActor))
		{

			CloseDoors();

			for (ABaseEnemy* Enemy : SpawnedEnemies)
			{
				Enemy->SetActorHiddenInGame(false);
				Enemy->SetActorTickEnabled(true);
				
				if (AAIController* Controller = Cast<AAIController>(Enemy->GetController()))
				{
					Controller->BrainComponent->StartLogic();
				}
			}

		
		}
		
	}
	
	for (ABaseItem* Item : SpawnedItems)
	{
		Item->SetActorHiddenInGame(false);
	}
	
	DeactivateTriggers();
	
}

void ABaseRoom::SpawnEnemies(int NumberOfEnemiesToSpawn)
{

	if (SpawnPoints.Num() < 0 || NumberOfEnemiesToSpawn < 0)
	{

		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, "No enemies were defined to spawn");
		
	}
	
	TArray<FName> RowNames = EnemyTable->GetRowNames();
	
	for (int iterator = 0; iterator < NumberOfEnemiesToSpawn; iterator++)
	{

		USceneComponent* spawnPoint = SpawnPoints[m_GameMode->RandomRangeInt(0, SpawnPoints.Num() - 1)];
		
		FActorSpawnParameters spawnInfo;

		FEnemyDataRow* enemyData = EnemyTable->FindRow<FEnemyDataRow>( RowNames[m_GameMode->RandomRangeInt(0, RowNames.Num() - 1)], nullptr, true ); 
		
		TObjectPtr<ABaseEnemy> newEnemy = GetWorld()->SpawnActorDeferred<ABaseEnemy>(
					enemyData->EnemyBP,
					spawnPoint->GetComponentTransform(),
					nullptr,
					nullptr);
		
		newEnemy->Initialize(enemyData->BaseHealth, enemyData->BaseDamage, enemyData->BaseRange);

		newEnemy->FinishSpawning(spawnPoint->GetComponentTransform(), false, nullptr);

		newEnemy->SetActorHiddenInGame(true);
		newEnemy->SetActorTickEnabled(false);
		
		if (AAIController* Controller = Cast<AAIController>(newEnemy->GetController()))
		{
			Controller->BrainComponent->StopLogic("Spawn");
		}
		
		newEnemy->OnEnemyKilled.AddDynamic(this, &ABaseRoom::EnemyKilled);

		SpawnedEnemies.Add(newEnemy);
		SpawnPoints.Remove(spawnPoint);
	}
	
}

void ABaseRoom::SpawnItems(int NumberOfItemsToSpawn)
{

	if (SpawnPoints.Num() < 0 || NumberOfItemsToSpawn < 0)
	{

		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, "No items were defined to spawn");
		
	}
	
	TArray<FName> RowNames = ItemTable->GetRowNames();
	
	for (int iterator = 0; iterator < NumberOfItemsToSpawn; iterator++)
	{
		
		USceneComponent* spawnPoint = SpawnPoints[m_GameMode->RandomRangeInt(0, SpawnPoints.Num() - 1)];
		
		FActorSpawnParameters spawnInfo;

		FItemDataRow* itemData = ItemTable->FindRow<FItemDataRow>( RowNames[m_GameMode->RandomRangeInt(0, RowNames.Num() - 1)], nullptr, true ); 
		
		TObjectPtr<ABaseItem> newItem = GetWorld()->SpawnActorDeferred<ABaseItem>(
					itemData->ItemBP,
					spawnPoint->GetComponentTransform(),
					nullptr,
					nullptr);
		
		newItem->ItemName = itemData->DisplayName;
		newItem->FinishSpawning(spawnPoint->GetComponentTransform(), false, nullptr);
		newItem->SetActorHiddenInGame(true);
		SpawnedItems.Add(newItem);
		SpawnPoints.Remove(spawnPoint);
	}
}

void ABaseRoom::SpawnCustom()
{
}

void ABaseRoom::OpenDoors()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Opening doors.......");
	
	for (ABaseDoor* door : Doors)
	{
		
		door->OpenDoor();
		
	}
	
}

void ABaseRoom::CloseDoors()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Closing doors.......");
	
	for (ABaseDoor* door : Doors)
	{
		
		door->CloseDoor();
		
	}
}

void ABaseRoom::EnemyKilled(ABaseEnemy* enemyKilled)
{

	SpawnedEnemies.Remove(enemyKilled);

	if (SpawnedEnemies.Num() == 0)
	{

		OpenDoors();
		
	}
	
}

void ABaseRoom::DeactivateTriggers()
{
	
	TArray<USceneComponent*> TriggerBoxes;
	TriggersFolder->GetChildrenComponents(false, TriggerBoxes);
	
	for (USceneComponent* trigger : TriggerBoxes)
	{

		if (UBoxComponent* boxColliderTrigger = Cast<UBoxComponent>(trigger))
		{
			
			boxColliderTrigger->OnComponentBeginOverlap.RemoveDynamic(this, &ABaseRoom::PlayerEnters);
			boxColliderTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
		}
		
	}
	
}
