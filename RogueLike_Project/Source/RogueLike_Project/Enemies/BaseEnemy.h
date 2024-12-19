// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Objects/Weapons/Projectiles/BaseProjectile.h"
#include "GameFramework/Character.h"
#include "RogueLike_Project/Components/HealthComponent.h"
#include "BaseEnemy.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* FirePoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|General Variables")
	TSubclassOf<ABaseProjectile> ProjectileType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|General Variables")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float BaseDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float Range;

public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

	virtual void SetActorTickEnabled(bool bEnabled) override;

	UFUNCTION(BlueprintCallable)
	virtual void Initialize(float NewMaxHealth, float NewDamage, float NewRange);

	UFUNCTION(BlueprintCallable)
	virtual void Fire();
	
	UFUNCTION(BlueprintCallable)
	void SetupProjectile();

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float oldHealth, float currentHealth, float normalizedHealth);

	UFUNCTION(BlueprintCallable)
	virtual void KillEnemy();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, ABaseEnemy*, enemyKilled);
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnEnemyKilled OnEnemyKilled;
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(Transient)
	TObjectPtr<ABaseProjectile>  newProjectile;
	
};
