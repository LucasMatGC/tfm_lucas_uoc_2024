// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	RootCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollider"));
	SetRootComponent(RootCollider);
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootCollider);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	ProjectileComponent->ProjectileGravityScale = 0;

}


// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	RootCollider->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileHit);
	
}

float ABaseProjectile::CalculateDamage()
{

	return (BaseDamage + AddedDamage) * DamageMultiplier;
	
}

void ABaseProjectile::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	
}

void ABaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor != nullptr)
	{
		
		//TODO: ultimo parametro define el tipo de daño (ej: electrico, fuego, hielo, etc)
		UGameplayStatics::ApplyDamage(OtherActor, CalculateDamage(), OwnerController, this, UDamageType::StaticClass());
		Destroy();
	
	}
	
}
