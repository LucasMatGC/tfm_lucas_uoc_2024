// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBoss.h"

#include "Kismet/GameplayStatics.h"
#include "Tasks/Task.h"

// Sets default values
ABaseBoss::ABaseBoss()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	FirePointPrimary = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointPrimary"));
	FirePointPrimary->SetupAttachment(GetMesh());
	
	FirePointSecondary = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointSecondary"));
	FirePointSecondary->SetupAttachment(GetMesh());
	
	FirePointSpecial = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointSpecial"));
	FirePointSpecial->SetupAttachment(GetMesh());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	PrimaryActorTick.bCanEverTick = true;
	
}

void ABaseBoss::Initialize(float NewMaxHealth, float NewDamage, float NewRange)
{

	HealthComponent->SetMaxHealth(NewMaxHealth);
	BaseDamage = NewDamage;
	Range = NewRange;
	
}

// Called when the game starts or when spawned
void ABaseBoss::BeginPlay()
{
	
	Super::BeginPlay();
	
	HealthComponent->OnUpdateCurrentHealth.AddDynamic(this, &ABaseBoss::TakeDamage);
	HealthComponent->OnProcessDeath.AddDynamic(this, &ABaseBoss::KillEnemy);
	
}

void ABaseBoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (HealthComponent != nullptr)
	{
		
		HealthComponent->OnUpdateCurrentHealth.RemoveDynamic(this, &ABaseBoss::TakeDamage);
		HealthComponent->OnProcessDeath.RemoveDynamic(this, &ABaseBoss::KillEnemy);
	
	}
	
	Super::EndPlay(EndPlayReason);
}

void ABaseBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseBoss::SetActorTickEnabled(bool bEnabled)
{
	Super::SetActorTickEnabled(bEnabled);

	this->Controller->SetActorTickEnabled(bEnabled);
}

void ABaseBoss::Fire(int AttackID)
{

	FActorSpawnParameters spawnInfo;

	switch (AttackID)
	{

		case 1:
			
			newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
				ProjectileType,
				FirePointPrimary->GetComponentTransform(),
				this,
				nullptr);

			SetupProjectile(false);
			
			newProjectile->FinishSpawning(FirePointPrimary->GetComponentTransform(), false, nullptr);

			newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
				ProjectileType,
				FirePointSecondary->GetComponentTransform(),
				this,
				nullptr);

			SetupProjectile(false);
				
			newProjectile->FinishSpawning(FirePointSecondary->GetComponentTransform(), false, nullptr);
			break;
		
		case 2:
			
			UseMeleeCollider(true);
			break;
		
		case 3:
			
			newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
				ProjectileType,
				FirePointSpecial->GetComponentTransform(),
				this,
				nullptr);

			SetupProjectile(true);
					
			newProjectile->FinishSpawning(FirePointSpecial->GetComponentTransform(), false, nullptr);
			break;

		default:
			break;

	}
	
}

void ABaseBoss::SetupProjectile(bool bIsSpecialProjectile)
{

	if (bIsSpecialProjectile)
	{

		newProjectile->ProjectileComponent->MaxSpeed = 0.f;
		newProjectile->ProjectileComponent->InitialSpeed = 0.f;
		newProjectile->BaseDamage = BaseDamage;
		
	}
	else
	{
		
		newProjectile->ProjectileComponent->MaxSpeed = 1000.f;
		newProjectile->ProjectileComponent->InitialSpeed = 1000.f;
		newProjectile->BaseDamage = BaseDamage;
		
	}
	
}

void ABaseBoss::UseMeleeCollider(bool isMeleeColliderActive)
{
	
	MeleeAttackMeshCollider->SetVisibility(isMeleeColliderActive, false);
	MeleeAttackMeshCollider->SetGenerateOverlapEvents(isMeleeColliderActive);
	
}

void ABaseBoss::ShootSpecial()
{

	newProjectile->ProjectileComponent->MaxSpeed = 5000.f;
	newProjectile->ProjectileComponent->InitialSpeed = 5000.f;
	
}

void ABaseBoss::TakeDamage(float oldHealth, float currentHealth, float normalizedHealth)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "OOOOOOUCH!!!!!!");
}

void ABaseBoss::KillEnemy() 
{

	OnBossKilled.Broadcast(this);
	
	// Hides visible components
	SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);

	// Stops the Actor from ticking
	SetActorTickEnabled(false);

	Destroy(true);

}
