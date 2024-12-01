// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRoom.h"

#include "BaseDoor.h"
#include "Chaos/DebugDrawQueue.h"
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

	//Spawn doors an all exits
	FActorSpawnParameters spawnInfo;
	TArray<USceneComponent*> Exits;
	ExitsFolder->GetChildrenComponents(false, Exits);
	for (USceneComponent* exit : Exits)
	{

		
		ABaseDoor* door = GetWorld()->SpawnActor<ABaseDoor>(
			DoorBP,
			exit->GetComponentTransform().GetLocation(),
			exit->GetComponentRotation(),
			spawnInfo);

		Doors.Add(door);
		door->AttachToComponent(exit, FAttachmentTransformRules::KeepWorldTransform, "Door");

		door->SetLocked(false);
		
	}

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
	if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(OtherActor))
	{

		CloseDoors();

		//TODO: Needs rework. Right now spawns enemies an ALL spawn points, as well as items. An enemy cannot spawn where an item has, and neither can do items where enemies have.
		
		if (ABaseGameMode* gameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode()))
		{

			seed = gameMode->RandomStream;
			
		}

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
				numberOfEnemiesToSpawn = seed.RandRange(0, SpawnPoints.Num());
				numberOfItemsToSpawn = SpawnPoints.Num() - numberOfEnemiesToSpawn;
				SpawnEnemies(numberOfEnemiesToSpawn);
				SpawnItems(numberOfItemsToSpawn);
				break;
			
			// Customized
			default:
				SpawnCustom();
				break;
				
		}

		DeactivateTriggers();
		
	}
	
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

		USceneComponent* spawnPoint = SpawnPoints[seed.RandRange(0, SpawnPoints.Num() - 1)];
		
		FActorSpawnParameters spawnInfo;

		FEnemyDataRow* enemyData = EnemyTable->FindRow<FEnemyDataRow>( RowNames[seed.RandRange(0, RowNames.Num() - 1)], nullptr, true ); 
		
		TObjectPtr<ABaseEnemy> newEnemy = GetWorld()->SpawnActorDeferred<ABaseEnemy>(
					enemyData->EnemyBP,
					spawnPoint->GetComponentTransform(),
					nullptr,
					nullptr);
		
		newEnemy->Initialize(enemyData->BaseHealth, enemyData->BaseDamage, enemyData->BaseRange);

		newEnemy->FinishSpawning(spawnPoint->GetComponentTransform(), false, nullptr);
		
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
		
		USceneComponent* spawnPoint = SpawnPoints[seed.RandRange(0, SpawnPoints.Num() - 1)];
		
		FActorSpawnParameters spawnInfo;

		FItemDataRow* itemData = ItemTable->FindRow<FItemDataRow>( RowNames[seed.RandRange(0, RowNames.Num() - 1)], nullptr, true ); 
		
		TObjectPtr<ABaseItem> newItem = GetWorld()->SpawnActorDeferred<ABaseItem>(
					itemData->ItemBP,
					spawnPoint->GetComponentTransform(),
					nullptr,
					nullptr);
		
		newItem->ItemName = itemData->DisplayName;
		newItem->FinishSpawning(spawnPoint->GetComponentTransform(), false, nullptr);
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
