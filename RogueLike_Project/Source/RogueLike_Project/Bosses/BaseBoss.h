// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Objects/Weapons/Projectiles/BaseProjectile.h"
#include "GameFramework/Character.h"
#include "RogueLike_Project/Components/HealthComponent.h"
#include "BaseBoss.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseBoss : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* FirePointPrimary;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* FirePointSecondary;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* FirePointSpecial;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* MeleeAttackMeshCollider;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|General Variables")
	TSubclassOf<ABaseProjectile> BaseProjectileType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|General Variables")
	TSubclassOf<ABaseProjectile> SpecialProjectileType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|General Variables")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float BaseDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float MeleeDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float SpecialDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float SpecialProjectileScale = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float SpecialProjectileWaitTime = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float Range;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss|General Variables", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float MeleeRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss|General Variables", meta = (ClampMin = "1", UIMin = "1"))
	int ProjectilesToFire;

public:	
	// Sets default values for this actor's properties
	ABaseBoss();

	virtual void SetActorTickEnabled(bool bEnabled) override;

	UFUNCTION(BlueprintCallable)
	virtual void Initialize(float NewMaxHealth, float NewDamage, float NewRange);

	UFUNCTION(BlueprintCallable)
	virtual void Fire(int AttackID);
	
	UFUNCTION(BlueprintCallable)
	void SetupProjectile(bool bIsSpecialProjectile);

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float oldHealth, float currentHealth, float maxHealth, float normalizedHealth);

	UFUNCTION(BlueprintCallable)
	virtual void KillEnemy(bool isMeleeDamage);

	UFUNCTION(BlueprintCallable)
	void UseMeleeCollider(bool isMeleeColliderActive);

	UFUNCTION(BlueprintCallable)
	void ApplyMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ShootSpecial();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossTrigger, bool, activateHealthBar);
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnBossTrigger OnBossTrigger;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetFeedback, bool, isVisible);
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnSetFeedback OnSetFeedback;;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossKilled, ABaseBoss*, bossKilled);
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnBossKilled OnBossKilled;
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(Transient)
	TObjectPtr<ABaseProjectile>  newProjectile;

private:

	bool m_bIsChargingSpecialProjectile = false;

	float m_CurrentSpecialProjectileWaitTime = 0.0f;
	
};
