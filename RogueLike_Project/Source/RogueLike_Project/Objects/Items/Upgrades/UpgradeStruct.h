#pragma once

#include "CoreMinimal.h"
#include "RogueLike_Project/Utils/Upgrades/BaseTrajectoryType.h"
#include "UpgradeStruct.generated.h"


/* Different types of Upgrades
 *	BaseVariablesUpgrade:	Upgrades that modify the base variables of the player or weapon, like MaxHealth, AddedDamage, DamageMultiplier, Range, FireRate, MaxAmmo, etc 
 *	DamageTypeUpgrade:		Upgrades that modify the damage type of the weapon, like Electric Damage, Fire Damage, Ice Damage or Explosive Damage
 *	ModifierUpgrade:		Upgrades that change the projectile itself, like the Mesh, the trajectory (linear, wavy, etc.), or the collision (bounciness, penetration, etc)
 *  CustomizedUpgrade:		Upgrades that change one or more variables or logic of the player, weapon or projectile. 
 */
UENUM(BlueprintType)
enum class EUpgradeType
{
	BaseVariablesUpgrade = 0,
	DamageTypeUpgrade = 1,
	ModifierUpgrade = 2,
	CustomizedUpgrade = 3
};

USTRUCT(BlueprintType)
struct ROGUELIKE_PROJECT_API FUpgradeStruct
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
	EUpgradeType UpgradeType = EUpgradeType::BaseVariablesUpgrade;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
	FName UpgradeName = FName("Upgrade Name");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
	FName UpgradeDescription = FName("Upgrade Description");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::BaseVariablesUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float ExtraHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::BaseVariablesUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::BaseVariablesUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedDamageMultiplier;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::BaseVariablesUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::BaseVariablesUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float ReducedFireRate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::BaseVariablesUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedMaxAmmo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::BaseVariablesUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float AddedMaxMagazine;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::DamageTypeUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	UDamageType* DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	UStaticMesh* NewProjectileMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	FCollisionProfileName NewCollisionProfile;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	UETrajectoryType NewTrajectoryType;
};