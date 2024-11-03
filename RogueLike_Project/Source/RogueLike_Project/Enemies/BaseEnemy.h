// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "../Items/Weapons/Projectiles/BaseProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "RogueLike_Project/Components/HealthComponent.h"
#include "BaseEnemy.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual bool CanFire() const;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* FirePoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|General Variables")
	TSubclassOf<ABaseProjectile> ProjectileType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|General Variables")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|General Variables")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|General Variables")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|General Variables")
	float BaseDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|General Variables")
	float MaxFireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|General Variables")
	float CurrentFireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|General Variables")
	float Range;

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void KillEnemy();
};
