﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual bool CanFire() const;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* RootScene;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|General Variables")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|General Variables")
	float MaxFireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|General Variables")
	float CurrentFireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|General Variables")
	float Range;

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void Reload();

	UFUNCTION(BlueprintCallable)
	virtual void DisableWeapon(bool toHide);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateAmmo, int, currentMagazine, int, remainingAmmo);
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FUpdateAmmo FUpdateAmmo;
};
