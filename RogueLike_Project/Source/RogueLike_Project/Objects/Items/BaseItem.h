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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;
	
	/** Health component **/
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractComponent* InteractComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::Consumable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float ExtraHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedDamage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedDamageMultiplier;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedRange;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float ReducedFireRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedMaxAmmo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::BaseVariablesUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedMaxMagazine;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::DamageTypeUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	UDamageType* DamageType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::ModifierUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	UStaticMesh* NewProjectileMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::ModifierUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	FCollisionProfileName NewCollisionProfile;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "ItemType == EItemType::ModifierUpgrade || ItemType == EItemType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	ETrajectoryType NewTrajectoryType;

public:
	
	// Sets default values for this actor's properties
	ABaseItem();
	
	UFUNCTION(BlueprintCallable)
	void PickUpItem();

	UFUNCTION(BlueprintCallable)
	void ApplyUpgrade(ABaseProjectile* projectile);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPickUpItem);
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FPickUpItem OnPickUpItem;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);

	virtual void Tick(float DeltaTime) override;

	virtual void HideItem();
};
