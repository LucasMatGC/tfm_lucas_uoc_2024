#pragma once

#include "CoreMinimal.h"
#include "UpgradeStruct.generated.h"


/* Different types of Upgrades
 *	BaseVariablesUpgrade:	Upgrades that modify the base variables of the player or weapon, like MaxHealth, AddedDamage, DamageMultiplier, Range, FireRate, MaxAmmo, etc 
 *	ModifierUpgrade:		Upgrades that change the projectile itself, like the Mesh, the trajectory (linear, wavy, etc.), or the collision (bounciness, penetration, etc)
 *  CustomizedUpgrade:		Upgrades that change one or more variables or logic of the player, weapon or projectile. 
 */
UENUM(BlueprintType)
enum class EUpgradeType
{
	BaseVariablesUpgrade = 0,
	ModifierUpgrade = 1,
	CustomizedUpgrade = 2
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
		meta = (EditCondition = "UpgradeType == EUpgradeType::BaseVariablesUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	int AddedCombo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::BaseVariablesUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float ExtraScale;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	UStaticMesh* NewProjectileMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	bool ChangeCollisionProfile = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	FCollisionProfileName NewCollisionProfile;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float LifeSteal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	float Bounciness;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	bool ChangeProjectileDestruction = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		meta = (EditCondition = "UpgradeType == EUpgradeType::ModifierUpgrade || UpgradeType == EUpgradeType::CustomizedUpgrade",
			EditConditionHides),
		Category = "Configuration|Upgrade")
	bool DestroyOnImpact = true;
};