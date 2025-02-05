﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class ROGUELIKE_PROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	UHealthComponent();
	
	void TakeDamage(float Damage, AActor* DamageCauser);

	void ProcessDeath(AActor* DamageCauser);

	void SetMaxHealth(float health);

	void SetHealth(float health);
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();
	
	UFUNCTION(BlueprintCallable)
	void UpgradeMaxHealth(float HealthUpgrade);
	
	UFUNCTION(BlueprintCallable)
	void Heal(float ConsumableHealth);
	
	UFUNCTION(BlueprintCallable)
	void UpdateHUD(float oldHealth);

	UFUNCTION(BlueprintCallable)
	void SetCanTakeDamage(bool CanTakeDamage);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FUpdateCurrentHealth, float, oldHealth, float, newHealth, float, maxHealth, float,
	                                               normalizedHealth);
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FUpdateCurrentHealth OnUpdateCurrentHealth;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProcessDeath, bool, isMeleeDamage);
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FProcessDeath OnProcessDeath;

	
protected:
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Health Values", meta = (ClampMin = "0", UIMin = "0"))
	float MaxHealth = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Health Values", meta = (ClampMin = "0", UIMin = "0"))
	float CurrentHealth = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Health Values")
	bool bCanTakeDamage = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Health Values")
	bool bPlayedDamageAnimation = false;
	
};
