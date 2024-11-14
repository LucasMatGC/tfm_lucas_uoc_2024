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

void ABaseRoom::SpawnEnemies()
{

	TArray<USceneComponent*> SpawnPoints;
	SpawnPointFolder->GetChildrenComponents(false, SpawnPoints);

	if (SpawnPoints.Num() > 0 || EnemiesToSpawn.Num() > 0)
	{

		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, "No enemies were defined to spawn");
		
	}

	FRandomStream seed;
	
	if (ABaseGameMode* gameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode()))
	{

		seed = gameMode->RandomStream;
		
	}
	
	TArray<FName> RowNames = EnemyRowHandle.DataTable->GetRowNames();
	
	for (USceneComponent* spawnPoint : SpawnPoints)
	{
		
		FActorSpawnParameters spawnInfo;

		FEnemyDataRow* enemyData = EnemyRowHandle.GetRow<FEnemyDataRow>( RowNames[seed.RandRange(0, RowNames.Num() - 1)].ToString() ); 
		
		TObjectPtr<ABaseEnemy> newEnemy = GetWorld()->SpawnActorDeferred<ABaseEnemy>(
					enemyData->EnemyBP,
					spawnPoint->GetComponentTransform(),
					nullptr,
					nullptr);
		
		newEnemy->Initialize(enemyData->BaseHealth, enemyData->BaseDamage, enemyData->BaseFireRate, enemyData->BaseRange);

		newEnemy->FinishSpawning(spawnPoint->GetComponentTransform(), false, nullptr);
		
		newEnemy->OnEnemyKilled.AddDynamic(this, &ABaseRoom::EnemyKilled);

		SpawnedEnemies.Add(newEnemy);
	}
	
}

void ABaseRoom::SpawnItems()
{

	TArray<USceneComponent*> SpawnPoints;
	SpawnPointFolder->GetChildrenComponents(false, SpawnPoints);

	if (SpawnPoints.Num() > 0 || EnemiesToSpawn.Num() > 0)
	{

		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, "No items were defined to spawn");
		
	}

	FRandomStream seed;
	
	if (ABaseGameMode* gameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode()))
	{

		seed = gameMode->RandomStream;
		
	}
	
	TArray<FName> RowNames = ItemRowHandle.DataTable->GetRowNames();
	
	for (USceneComponent* spawnPoint : SpawnPoints)
	{
		
		FActorSpawnParameters spawnInfo;

		FItemDataRow* itemData = ItemRowHandle.GetRow<FItemDataRow>( RowNames[seed.RandRange(0, RowNames.Num() - 1)].ToString() ); 
		
		TObjectPtr<ABaseItem> newItem = GetWorld()->SpawnActorDeferred<ABaseItem>(
					itemData->ItemBP,
					spawnPoint->GetComponentTransform(),
					nullptr,
					nullptr);

		newItem->FinishSpawning(spawnPoint->GetComponentTransform(), false, nullptr);
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
		
		if (Functionality == ERoomFunctionality::RandomEnemies || Functionality == ERoomFunctionality::FullyRandom)
		{
			SpawnEnemies();
		}
		if (Functionality == ERoomFunctionality::RandomItems || Functionality == ERoomFunctionality::FullyRandom)
		{
			SpawnItems();
		}
		if (Functionality == ERoomFunctionality::Customized)
		{
			SpawnCustom();
		}

		DeactivateTriggers();
		
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
