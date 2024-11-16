// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "RogueLike_Project/Components/InteractComponent.h"
#include "RogueLike_Project/Objects/Weapons/Projectiles/BaseProjectile.h"
#include "RogueLike_Project/Utils/Upgrades/BaseTrajectoryType.h"
#include "BaseItem.generated.h"

/* Different types of items
 *	Consumable:				Items that are instantly use to refill the player's health or ammunition
 *	BaseVariableUpgrade:	Upgrades that modify the base variables of the player or weapon, like MaxHealth, AddedDamage, DamageMultiplier, Range, FireRate, MaxAmmo, etc 
 *	DamageTypeUpgrade:		Upgrades that modify the damage type of the weapon, like Electric Damage, Fire Damage, Ice Damage or Explosive Damage
 *	ModifierUpgrade:		Upgrades that change the projectile itself, like the Mesh, the trayectory (linear, wavy, etc), or the collision (bounciness, penetration, etc)
 *  CustomizedUpgrade:		Upgrades that change one or more variables or logic of the player, weapon or projectile. 
 */
UENUM(BlueprintType)
enum class EItemType
{
	Consumable = 0,
	BaseVariablesUpgrade = 1,
	DamageTypeUpgrade = 2,
	ModifierUpgrade = 3,
	CustomizedUpgrade = 4
};

UCLASS()
class ROGUELIKE_PROJECT_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* RootCollider;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;
	
	/** Health component **/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UInteractComponent* InteractComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
	FName ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemType ItemType = EItemType::Consumable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::Consumable", EditConditionHides), Category = "Configuration|Upgrade")
	float ConsumableHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::Consumable", EditConditionHides), Category = "Configuration|Upgrade")
	float ConsumableAmmo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float ExtraHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedDamageMultiplier;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float ReducedFireRate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedMaxAmmo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedMaxMagazine;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::DamageTypeUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	UDamageType* DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::ModifierUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	UStaticMesh* NewProjectileMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::ModifierUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	FCollisionProfileName NewCollisionProfile;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::ModifierUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	ETrajectoryType NewTrajectoryType;

public:
	
	// Sets default values for this actor's properties
	ABaseItem();
	
	UFUNCTION(BlueprintCallable)
	void PickUpItem(ACharacter* interactor);

	UFUNCTION(BlueprintCallable)
	void ApplyUpgrade(ABaseProjectile* projectile);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUseConsumable, float, health, float, ammo);
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnUseConsumable OnUseConsumable;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);

	virtual void Tick(float DeltaTime) override;

	virtual void HideItem();
};
