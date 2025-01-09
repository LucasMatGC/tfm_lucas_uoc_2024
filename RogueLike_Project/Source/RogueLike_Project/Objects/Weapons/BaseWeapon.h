// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueLike_Project/Objects/Items/Upgrades/UpgradeStruct.h"
#include "Sound/SoundCue.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables")
	FName WeaponName;
	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Range;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float LifeSteal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|General Variables")
	USceneComponent* FirePoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|General Variables")
	TArray<FUpgradeStruct> CommonUpgrades;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|General Variables")
	TArray<FUpgradeStruct> Upgrades;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|General Variables")
	USoundCue* FireSFX;

public:
	
	ABaseWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void SetUpWeapon(TArray<FUpgradeStruct> NewCommonUpgrades);

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void Reload();
	
	UFUNCTION(BlueprintCallable)
	virtual void UpdateHUD();

	UFUNCTION(BlueprintCallable)
	virtual void DisableWeapon(bool toHide);
	
	UFUNCTION(BlueprintCallable)
	virtual void AddUpgrade(FUpgradeStruct newUpgrade, bool bIsCommonUpgrade);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateAmmo, int, currentMagazine, int, remainingAmmo);
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FUpdateAmmo FUpdateAmmo;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEmptyMagazineShoot);
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnEmptyMagazineShoot OnEmptyMagazineShoot;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponFire, bool ,bIsMeleeWeapon, int, CurrentCombo);
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponFire OnWeaponFire;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual bool CanFire() const;

	virtual void ApplyUpgrade(const FUpgradeStruct& Upgrade);
	
};
