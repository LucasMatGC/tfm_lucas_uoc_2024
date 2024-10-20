// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
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

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Variables")
	float MaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Variables")
	float CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Variables")
	float MaxMagazine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranged Variables")
	float CurrentMagazine;

	virtual void Fire() override;

	UFUNCTION(BlueprintCallable)
	virtual void Reload();
};
