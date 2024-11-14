// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "RogueLike_Project/Objects/Items/BaseItem.h"
#include "BaseWeapon.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* RootScene;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables", meta = (ClampMin = "1", UIMin = "1"))
	float BaseDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables")
	float AddedDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float DamageMultiplier = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float MaxFireRate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables")
	float CurrentFireRate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float Range;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables")
	TArray<TObjectPtr<ABaseItem>> Upgrades;

public:
	
	ABaseWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void Reload();

	UFUNCTION(BlueprintCallable)
	virtual void DisableWeapon(bool toHide);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateAmmo, int, currentMagazine, int, remainingAmmo);
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FUpdateAmmo FUpdateAmmo;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual bool CanFire() const;
};
