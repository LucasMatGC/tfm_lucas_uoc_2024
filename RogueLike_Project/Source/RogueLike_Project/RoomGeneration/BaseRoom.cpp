// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRoom.h"

#include "AIController.h"
#include "BaseDoor.h"
#include "BrainComponent.h"
#include "Components/BoxComponent.h"
#include "RogueLike_Project/RogueLike_ProjectCharacter.h"
#include "RogueLike_Project/GameModes/GameplayGameMode.h"

// Sets default values
ABaseRoom::ABaseRoom()
{

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	ExitsFolder = CreateDefaultSubobject<USceneComponent>(TEXT("ExitsFolder"));
	GeometryFolder = CreateDefaultSubobject<USceneComponent>(TEXT("GeometryFolder"));
	OverlapFolder = CreateDefaultSubobject<USceneComponent>(TEXT("OverlapFolder"));
	SpawnPointFolder = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPointFolder"));
	ItemsFolder = CreateDefaultSubobject<USceneComponent>(TEXT("ItemsFolder"));
	TriggersFolder = CreateDefaultSubobject<USceneComponent>(TEXT("TriggersFolder"));
	TeleportsFolder = CreateDefaultSubobject<USceneComponent>(TEXT("TeleportsFolder"));
	Direction = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"));

	SetRootComponent(RootScene);
	GeometryFolder->SetupAttachment(RootScene);
	ExitsFolder->SetupAttachment(RootScene);
	OverlapFolder->SetupAttachment(RootScene);
	SpawnPointFolder->SetupAttachment(RootScene);
	ItemsFolder->SetupAttachment(RootScene);
	TriggersFolder->SetupAttachment(RootScene);
	TeleportsFolder->SetupAttachment(RootScene);
	Direction->SetupAttachment(RootScene);

}

// Prepare the room depending on the room type
void ABaseRoom::PrepareRoom(AGameplayGameMode* GameMode)
{

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
	ItemsFolder->GetChildrenComponents(false, ItemSpawnPoints);

	m_GameMode = GameMode;
	
	int numberOfEnemiesToSpawn = 0;
	int numberOfItemsToSpawn = 0;

	switch (Functionality)
	{
		
		// Spawn all enemies 
		case ERoomFunctionality::RandomEnemies:
			SpawnEnemies(SpawnPoints.Num());
			break;

		// Spawn all items
		case ERoomFunctionality::RandomItems:
			SpawnItems(ItemSpawnPoints.Num());
			break;

		// Spawn Boss
		case ERoomFunctionality::RandomBoss:
			SpawnBoss();
			break;

		// Spawn enemies and items. The number of both is randomized
		case ERoomFunctionality::FullyRandom:
			numberOfEnemiesToSpawn = m_GameMode->RandomRangeInt(0, SpawnPoints.Num());
			numberOfItemsToSpawn = FMath::Min(SpawnPoints.Num() - numberOfEnemiesToSpawn, ItemSpawnPoints.Num());
			SpawnEnemies(numberOfEnemiesToSpawn);
			SpawnItems(numberOfItemsToSpawn);
			break;

		// Spawn the start items
		case ERoomFunctionality::StartRoom:
			SpawnItems(ItemSpawnPoints.Num());
			for (ABaseItem* Item : SpawnedItems)
			{
				Item->SetActorHiddenInGame(false);
			}
			break;
				
		// Customized
		default:
			SpawnCustom();
			break;
				
	}
	
}

// Creates a door in the specified location. Locks the door if necessary.
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

	EnemyTable.LoadSynchronous();
	BossTable.LoadSynchronous();
	ItemTable.LoadSynchronous();
	
}

// Called when the game ends or when despawned
void ABaseRoom::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TArray<USceneComponent*> TeleportPoints;
	TeleportsFolder->GetChildrenComponents(false, TeleportPoints);
	for (USceneComponent* teleport : TeleportPoints)
	{

		if (UStaticMeshComponent* Cylinder = Cast<UStaticMeshComponent>(teleport))
		{
			
			Cylinder->OnComponentBeginOverlap.RemoveDynamic(this, &ABaseRoom::ExitLevel);
			
		}
		
	}
}

// Called every frame
void ABaseRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Trigger logic when player enters the room. 
void ABaseRoom::PlayerEnters(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(OtherActor))
	{

		// If Enemies exists in the room, close doors, activate and show them
		if (SpawnedEnemies.Num() > 0)
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

		// If Boss exist in the room, close doors, activate and show it
		if (SpawnedBoss != nullptr)
		{
		
			CloseDoors();
			
			SpawnedBoss->SetActorHiddenInGame(false);
			SpawnedBoss->SetActorTickEnabled(true);
			SpawnedBoss->OnBossTrigger.Broadcast(true);
				
			if (AAIController* Controller = Cast<AAIController>(SpawnedBoss->GetController()))
			{
				Controller->BrainComponent->StartLogic();
			}
		
		}

		// Show spawned items
		for (ABaseItem* Item : SpawnedItems)
		{
			Item->SetActorHiddenInGame(false);
		}
		
	}
	
	DeactivateTriggers();
	
}

// Change Level to the next one. Logic handled by GameMode
void ABaseRoom::ExitLevel(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(OtherActor))
	{

		if (AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode()))
		{

			GameMode->LoadNextLevel();
			
		}
		
	}
}

// Spawn enemies in the specified spawn points. Stop logic of enemies until player enters the room
void ABaseRoom::SpawnEnemies(int NumberOfEnemiesToSpawn)
{

	if (SpawnPoints.Num() < 0 || NumberOfEnemiesToSpawn < 0)
	{

		UE_LOG(LogSpawn, Error, TEXT("No enemies were defined to spawn"));
		
	}
	
	EnemyTable.LoadSynchronous();
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
		
		newEnemy->Initialize(enemyData->BaseHealth, enemyData->BaseDamage, enemyData->BaseRange, m_GameMode->RandomRangeFloat(0.0f, 1.0f));

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

// Spawn items in the specified spawn points. Hides items until player enters the room
void ABaseRoom::SpawnItems(int NumberOfItemsToSpawn)
{

	if (ItemSpawnPoints.Num() < 0 || NumberOfItemsToSpawn < 0)
	{

		UE_LOG(LogSpawn, Error, TEXT("No items were defined to spawn"));
		
	}
	
	ItemTable.LoadSynchronous();
	TArray<FName> RowNames = ItemTable->GetRowNames();
	
	for (int iterator = 0; iterator < NumberOfItemsToSpawn; iterator++)
	{
		
		USceneComponent* spawnPoint = ItemSpawnPoints[m_GameMode->RandomRangeInt(0, ItemSpawnPoints.Num() - 1)];
		
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
		ItemSpawnPoints.Remove(spawnPoint);
	}
}

// Spawn a boss in the specified spawn point. Stop logic of boss until player enters the room
void ABaseRoom::SpawnBoss()
{
	
	BossTable.LoadSynchronous();
	TArray<FName> RowNames = BossTable->GetRowNames();
	
	USceneComponent* spawnPoint = SpawnPoints[m_GameMode->RandomRangeInt(0, SpawnPoints.Num() - 1)];
	
	FActorSpawnParameters spawnInfo;

	FBossDataRow* bossData = BossTable->FindRow<FBossDataRow>( RowNames[m_GameMode->RandomRangeInt(0, RowNames.Num() - 1)], nullptr, true ); 
	
	SpawnedBoss = GetWorld()->SpawnActorDeferred<ABaseBoss>(
				bossData->BossBP,
				spawnPoint->GetComponentTransform(),
				nullptr,
				nullptr);

	SpawnedBoss->FinishSpawning(spawnPoint->GetComponentTransform(), false, nullptr);

	SpawnedBoss->SetActorHiddenInGame(true);
	SpawnedBoss->SetActorTickEnabled(false);
		
	if (AAIController* Controller = Cast<AAIController>(SpawnedBoss->GetController()))
	{
		Controller->BrainComponent->StopLogic("Spawn");
	}
		
	SpawnedBoss->OnBossKilled.AddDynamic(this, &ABaseRoom::BossKilled);

	SpawnPoints.Remove(spawnPoint);
	
}

// Logic for custom rooms with special logic
void ABaseRoom::SpawnCustom()
{
}

// Open all unlocked doors
void ABaseRoom::OpenDoors()
{
	
	for (ABaseDoor* door : Doors)
	{
		
		door->OpenDoor();
		
	}
	
}

// Close all doors without locking them
void ABaseRoom::CloseDoors()
{
	
	for (ABaseDoor* door : Doors)
	{
		
		door->CloseDoor();
		
	}
}

// Called when an enemy is killed. If all enemies are killed, open doors
void ABaseRoom::EnemyKilled(ABaseEnemy* enemyKilled, bool isMeleeDamage)
{

	SpawnedEnemies.Remove(enemyKilled);

	m_GameMode->CheckItemSpawn(enemyKilled->RandomizedItemSpawnRate, enemyKilled->GetActorTransform(), isMeleeDamage);

	if (SpawnedEnemies.Num() == 0)
	{

		OpenDoors();
		
	}
	
}

// Called when boss of the room is killed. Spawns loot, open door and enable levels exit
void ABaseRoom::BossKilled(ABaseBoss* BossKilled)
{

	OpenDoors();

	SpawnItems(ItemSpawnPoints.Num());
	
	for (ABaseItem* Item : SpawnedItems)
	{
		Item->SetActorHiddenInGame(false);
	}

	EnableExitLevel();
	
}

// Activate the exit of the room
void ABaseRoom::EnableExitLevel()
{
	TArray<USceneComponent*> TeleportPoints;
	TeleportsFolder->GetChildrenComponents(false, TeleportPoints);
	
	for (USceneComponent* teleport : TeleportPoints)
	{

		if (UStaticMeshComponent* Cylinder = Cast<UStaticMeshComponent>(teleport))
		{
			
			Cylinder->OnComponentBeginOverlap.AddDynamic(this, &ABaseRoom::ExitLevel);
			Cylinder->SetVisibility(true);
			Cylinder->SetGenerateOverlapEvents(true);
			
		}
		
	}
}

// Deactivate all player triggers in the room
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
