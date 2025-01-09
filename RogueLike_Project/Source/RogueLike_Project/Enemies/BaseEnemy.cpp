// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(GetMesh());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	PrimaryActorTick.bCanEverTick = true;
	
}

void ABaseEnemy::Initialize(float NewMaxHealth, float NewDamage, float NewRange, float NewRandomizedItemSpawnRate)
{

	HealthComponent->SetMaxHealth(NewMaxHealth);
	BaseDamage = NewDamage;
	Range = NewRange;
	RandomizedItemSpawnRate = NewRandomizedItemSpawnRate;
	
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	
	Super::BeginPlay();
	
	HealthComponent->OnUpdateCurrentHealth.AddDynamic(this, &ABaseEnemy::TakeDamage);
	HealthComponent->OnProcessDeath.AddDynamic(this, &ABaseEnemy::KillEnemy);
	
}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (HealthComponent != nullptr)
	{
		
		HealthComponent->OnUpdateCurrentHealth.RemoveDynamic(this, &ABaseEnemy::TakeDamage);
		HealthComponent->OnProcessDeath.RemoveDynamic(this, &ABaseEnemy::KillEnemy);
	
	}
	
	Super::EndPlay(EndPlayReason);
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::SetActorTickEnabled(bool bEnabled)
{
	Super::SetActorTickEnabled(bEnabled);

	//this->Controller->SetActorTickEnabled(bEnabled);
}

void ABaseEnemy::Fire()
{

	FActorSpawnParameters spawnInfo;
	
	newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
		ProjectileType,
		FirePoint->GetComponentTransform(),
		this,
		nullptr);

	SetupProjectile();
	
	newProjectile->FinishSpawning(FirePoint->GetComponentTransform(), false, nullptr);
	
	UGameplayStatics::SpawnSound2D(this, FireSFX);
	
}

void ABaseEnemy::SetupProjectile()
{
	
	newProjectile->ProjectileComponent->MaxSpeed = 1000.f;
	newProjectile->ProjectileComponent->InitialSpeed = 1000.f;
	newProjectile->BaseDamage = BaseDamage;
	
}

void ABaseEnemy::TakeDamage(float oldHealth, float currentHealth, float maxHealth, float normalizedHealth)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "OOOOOOUCH!!!!!!");
}

void ABaseEnemy::KillEnemy(bool isMeleeDamage) 
{

	OnEnemyKilled.Broadcast(this, isMeleeDamage);

	
	if (AAIController* AIController = Cast<AAIController>(this->GetController()))
	{
		AIController->BrainComponent->StopLogic("Death");
	}
	
	UGameplayStatics::SpawnSound2D(this, DeathSFX);
	
	// Hides visible components
	SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);

	// Stops the Actor from ticking
	SetActorTickEnabled(false);

	Destroy(true);

}
