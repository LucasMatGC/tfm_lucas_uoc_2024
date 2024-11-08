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
	int MaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	int CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	int MaxMagazine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	int CurrentMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	USceneComponent* FirePoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ranged Variables")
	TSubclassOf<ABaseProjectile> ProjectileType;
	
	virtual void Fire() override;
	
	virtual void Reload() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetMaxAmmo();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetMaxMagazine();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetCurrentAmmo();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int GetCurrentMagazine();

private:
	
	static void SetupProjectile(TObjectPtr<ABaseProjectile> newProjectile);
};
