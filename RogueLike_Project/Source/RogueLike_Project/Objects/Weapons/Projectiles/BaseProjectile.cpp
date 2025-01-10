// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "RogueLike_Project/RogueLike_ProjectCharacter.h"
#include "RogueLike_Project/Bosses/BaseBoss.h"
#include "RogueLike_Project/Enemies/BaseEnemy.h"

class ARogueLike_ProjectCharacter;
// Sets default values
ABaseProjectile::ABaseProjectile()
{

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);
	RootCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollider"));
	RootCollider->SetupAttachment(ProjectileMesh);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	ProjectileComponent->ProjectileGravityScale = 0;

}


// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileHit);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnProjectileOverlap);
	
}

// Calculate damage of projectile
float ABaseProjectile::CalculateDamage() const
{

	return (BaseDamage + AddedDamage) * DamageMultiplier;
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	
}

// Called when projectile hits other objects
void ABaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor != nullptr)
	{
		// Apply damage to other actore
		UGameplayStatics::ApplyDamage(OtherActor, CalculateDamage(), OwnerController, this, UDamageType::StaticClass());

		// If Projectile has lifesteal and other actor is an enemy or boss, heal player specified ammount
		if(LifeSteal > 0.0)
		{
			if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor))
			{
				if (ARogueLike_ProjectCharacter* Player = Cast<ARogueLike_ProjectCharacter>(OwnerController->GetCharacter()))
				{
					float HealAmmount = FMathf::Floor(LifeSteal * CalculateDamage());
		
					Player->HealPlayer(HealAmmount);	
				}
			}
			else if (ABaseBoss* Boss = Cast<ABaseBoss>(OtherActor))
			{
				if (ARogueLike_ProjectCharacter* Player = Cast<ARogueLike_ProjectCharacter>(OwnerController->GetCharacter()))
				{
					float HealAmmount = FMathf::Floor(LifeSteal * CalculateDamage());
		
					Player->HealPlayer(HealAmmount);	
				}
			}
		}

		// Destory projectile if needed
		if (bDestroyOnImpact)
		{
			
			Destroy();
			
		}
	
	}
	
}

// Called when projectile overlaps other objects
void ABaseProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		// Apply damage to other actore
		UGameplayStatics::ApplyDamage(OtherActor, CalculateDamage(), OwnerController, this, UDamageType::StaticClass());

		// If Projectile has lifesteal and other actor is an enemy or boss, heal player specified ammount
		if(LifeSteal > 0.0)
		{
			if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor))
			{
				if (ARogueLike_ProjectCharacter* Player = Cast<ARogueLike_ProjectCharacter>(OwnerController->GetCharacter()))
				{
					float HealAmmount = FMathf::Floor(LifeSteal * CalculateDamage());
		
					Player->HealPlayer(HealAmmount);	
				}
			}
			else if (ABaseBoss* Boss = Cast<ABaseBoss>(OtherActor))
			{
				if (ARogueLike_ProjectCharacter* Player = Cast<ARogueLike_ProjectCharacter>(OwnerController->GetCharacter()))
				{
					float HealAmmount = FMathf::Floor(LifeSteal * CalculateDamage());
		
					Player->HealPlayer(HealAmmount);	
				}
			}
		}
		
		// Destory projectile if needed
		if (bDestroyOnImpact)
		{
			
			Destroy();
			
		}
	
	}
}
