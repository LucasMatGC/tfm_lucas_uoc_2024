// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee Variables", meta = (ClampMin = "1", UIMin = "1"))
	int MaxCombo = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee Variables")
	int CurrentCombo = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float MaxComboRate = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee Variables")
	float CurrentComboRate = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee Variables")
	float AttackAngle = 90.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee Variables")
	bool bIsAttacking = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee Variables")
	bool bCanDoComboAttack = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee Variables")
	UStaticMesh* AttackMesh;

public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

	virtual void Fire() override;
	
	UFUNCTION(BlueprintCallable)
	void HideAttackCollision();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(AActor* OtherActor);

	virtual void UpdateHUD() override;

	virtual void AddUpgrade(FUpgradeStruct newUpgrade, bool bIsCommonUpgrade) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	float CalculateDamage() const;
	
};
