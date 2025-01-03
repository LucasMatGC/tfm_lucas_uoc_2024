// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseProjectile.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration", meta = (ClampMin = "1", UIMin = "1"))
	float BaseDamage = 20.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration")
	float AddedDamage = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float DamageMultiplier = 1.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float LifeSteal = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration")
	bool bDestroyOnImpact = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration")
	AController* OwnerController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration")
	TObjectPtr<UDamageType> DamageType;
	
public:
	// Sets default values for this actor's properties
	ABaseProjectile();
	
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UFUNCTION()
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float CalculateDamage() const;

public:
	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* RootCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ProjectileMesh;
	
	/** Projectile movement component */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = ( AllowPrivateAccess = "true" ))
	UProjectileMovementComponent* ProjectileComponent;
};
