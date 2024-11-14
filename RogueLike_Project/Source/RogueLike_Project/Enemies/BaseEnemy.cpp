// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

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
	
	if (CurrentFireRate > 0)
	{

		CurrentFireRate -= DeltaTime;
		
	}
	else
	{
		Fire();
	}
}

bool ABaseEnemy::CanFire() const
{

	return CurrentFireRate <= 0;
	
}

void ABaseEnemy::Fire()
{

	if (CanFire())
	{

		CurrentFireRate = MaxFireRate;
		
		FActorSpawnParameters spawnInfo;
		
		TObjectPtr<ABaseProjectile> newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
			ProjectileType,
			FirePoint->GetComponentTransform(),
			//TODO: Definir como owner el player.
			nullptr,
			nullptr);
		
		newProjectile->FinishSpawning(FirePoint->GetComponentTransform(), false, nullptr);

		//UGameplayStatics::ApplyDamage(this, 10, GetController(), this, UDamageType::StaticClass());
		
	}	
	
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
