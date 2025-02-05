﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBoss.h"

#include "Kismet/GameplayStatics.h"
#include "RogueLike_Project/GameModes/GameplayGameMode.h"
#include "Tasks/Task.h"

// Sets default values
ABaseBoss::ABaseBoss()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	// Defines the Fire point of the weapons (Spawn point for projectiles)
	FirePointPrimary = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointPrimary"));
	FirePointPrimary->SetupAttachment(GetMesh());
	
	FirePointSecondary = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointSecondary"));
	FirePointSecondary->SetupAttachment(GetMesh());
	
	// Defines the Fire point of the special weapons (Spawn point for special projectiles)
	FirePointSpecial = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointSpecial"));
	FirePointSpecial->SetupAttachment(GetMesh());

	MeleeAttackMeshCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeAttackMeshCollider"));
	MeleeAttackMeshCollider->SetupAttachment(GetMesh());
	MeleeAttackMeshCollider->SetHiddenInGame(true);
	MeleeAttackMeshCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	PrimaryActorTick.bCanEverTick = true;
	
}

// Sets base variables
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
	MeleeAttackMeshCollider->OnComponentBeginOverlap.AddDynamic(this, &ABaseBoss::ApplyMeleeDamage);
	
}

// Called when the game ends or when despawned
void ABaseBoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (HealthComponent != nullptr)
	{
		
		HealthComponent->OnUpdateCurrentHealth.RemoveDynamic(this, &ABaseBoss::TakeDamage);
		HealthComponent->OnProcessDeath.RemoveDynamic(this, &ABaseBoss::KillEnemy);
		MeleeAttackMeshCollider->OnComponentBeginOverlap.RemoveDynamic(this, &ABaseBoss::ApplyMeleeDamage);
	
	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ABaseBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bIsChargingSpecialProjectile)
	{

		if (m_CurrentSpecialProjectileWaitTime > 0.0)
		{
			m_CurrentSpecialProjectileWaitTime -= DeltaTime;

			if (m_CurrentSpecialProjectileWaitTime <= 0.0)
			{

				m_bIsChargingSpecialProjectile = false;
				
			}
		}

		float scale = FMath::Lerp(1.0f, SpecialProjectileScale, (SpecialProjectileWaitTime - m_CurrentSpecialProjectileWaitTime) / SpecialProjectileWaitTime);
		
		FVector NewScale = FVector(scale , scale, scale);
		newProjectile->ProjectileMesh->SetRelativeScale3D(NewScale);
		
	}
	
}

// Enable tick of actor and controller
void ABaseBoss::SetActorTickEnabled(bool bEnabled)
{
	Super::SetActorTickEnabled(bEnabled);

	this->Controller->SetActorTickEnabled(bEnabled);
}

// Fires the specified attack of the boss
void ABaseBoss::Fire(int AttackID)
{

	FActorSpawnParameters spawnInfo;

	switch (AttackID)
	{
		// ID 1: Basic attack. Shoots one projectile from each base fire point
		case 1:
			
			newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
				BaseProjectileType,
				FirePointPrimary->GetComponentTransform(),
				this,
				nullptr);

			SetupProjectile(false);
			
			newProjectile->FinishSpawning(FirePointPrimary->GetComponentTransform(), false, nullptr);

			newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
				BaseProjectileType,
				FirePointSecondary->GetComponentTransform(),
				this,
				nullptr);

			SetupProjectile(false);
				
			newProjectile->FinishSpawning(FirePointSecondary->GetComponentTransform(), false, nullptr);
		
			UGameplayStatics::SpawnSound2D(this, BaseFireSFX);
			break;

		// ID 2: Activate melee collider
		case 2:
			
			UseMeleeCollider(true);
			break;

		// ID 3: Prepares special projectile
		case 3:
			
			newProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
				SpecialProjectileType,
				FirePointSpecial->GetComponentTransform(),
				this,
				nullptr);

			SetupProjectile(true);
					
			newProjectile->FinishSpawning(FirePointSpecial->GetComponentTransform(), false, nullptr);
			break;

	}
	
}

// Initializes projectile variables
void ABaseBoss::SetupProjectile(bool bIsSpecialProjectile)
{
	
	newProjectile->SetLifeSpan(1000.0f);

	// If projectiles is a special projectile, attach it to parent while it charges and set to 0 speed
	if (bIsSpecialProjectile)
	{

		newProjectile->BaseDamage = SpecialDamage;
		newProjectile->ProjectileComponent->MaxSpeed = 1500.f;
		newProjectile->ProjectileComponent->InitialSpeed = 0.f;
		newProjectile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "FirePointSpecial");
		
		m_bIsChargingSpecialProjectile = true;
		m_CurrentSpecialProjectileWaitTime = SpecialProjectileWaitTime;
		
	}
	else
	{
		
		newProjectile->BaseDamage = BaseDamage;
		newProjectile->ProjectileComponent->MaxSpeed = 1000.f;
		newProjectile->ProjectileComponent->InitialSpeed = 1000.f;
		
	}
	
}

// Activate / Deactivate melee collider
void ABaseBoss::UseMeleeCollider(bool isMeleeColliderActive)
{
	
	MeleeAttackMeshCollider->SetVisibility(isMeleeColliderActive, false);
	MeleeAttackMeshCollider->SetGenerateOverlapEvents(isMeleeColliderActive);
	UGameplayStatics::SpawnSound2D(this, MeleeAttackSFX);
	
}

// Apply melee damage to overlaped actor
void ABaseBoss::ApplyMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UGameplayStatics::ApplyDamage(OtherActor, MeleeDamage, GetWorld()->GetFirstPlayerController(), this, UDamageType::StaticClass());
	
}

// Detach and fire special projectile
void ABaseBoss::ShootSpecial()
{

	if (newProjectile != nullptr)
	{
		UGameplayStatics::SpawnSound2D(this, SpecialFireSFX);
		newProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		newProjectile->ProjectileComponent->InitialSpeed = 2000.f;
		newProjectile->ProjectileComponent->Velocity = newProjectile->GetActorForwardVector().GetSafeNormal() * newProjectile->ProjectileComponent->MaxSpeed;
		
	}
	
}


// Function called after damage logic
void ABaseBoss::TakeDamage(float oldHealth, float currentHealth, float maxHealth, float normalizedHealth)
{
}

// Deactivate boss and notifies of boss killed
void ABaseBoss::KillEnemy(bool isMeleeDamage) 
{

	OnBossTrigger.Broadcast(false);
	OnBossKilled.Broadcast(this);
	UGameplayStatics::SpawnSound2D(this, DeathSFX);
	
	// Hides visible components
	SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);

	// Stops the Actor from ticking
	SetActorTickEnabled(false);

	Destroy(true);

}
