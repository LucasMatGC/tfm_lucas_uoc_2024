// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/Actor.h"
#include "Projectiles/BaseProjectile.h"
#include "RangeWeapon.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ARangeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangeWeapon();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual bool CanFire() const override;

	virtual bool CanReload();

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	float MaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	float CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	float MaxMagazine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	float CurrentMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	const UStaticMeshSocket* FirePoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	TSubclassOf<ABaseProjectile> ProjectileType;

	virtual void Fire() override;

	UFUNCTION(BlueprintCallable)
	virtual void Reload();

private:
	
	static void SetupProjectile(TObjectPtr<ABaseProjectile> newProjectile);
};
