// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(GetMesh());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	PrimaryActorTick.bCanEverTick = true;
	
}

void ABaseEnemy::Initialize(float NewMaxHealth, float NewDamage, float NewMaxFireRate, float NewRange)
{

	HealthComponent->SetMaxHealth(NewMaxHealth);
	BaseDamage = NewDamage;
	MaxFireRate = NewMaxFireRate;
	Range = NewRange;
	
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

bool ABaseEnemy::CanFire() const
{

	return true;
	
}

void ABaseEnemy::Fire()
{
}

void ABaseEnemy::TakeDamage(float oldHealth, float currentHealth, float normalizedHealth)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "OOOOOOUCH!!!!!!");
}

void ABaseEnemy::KillEnemy() 
{

	OnEnemyKilled.Broadcast(this);
	
	// Hides visible components
	SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);

	// Stops the Actor from ticking
	SetActorTickEnabled(false);

	Destroy(true);

}
