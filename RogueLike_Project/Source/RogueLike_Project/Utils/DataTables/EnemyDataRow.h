#pragma once

#include "CoreMinimal.h"
#include "RogueLike_Project/Enemies/BaseEnemy.h"
#include "EnemyDataRow.generated.h"

USTRUCT(BlueprintType)
struct ROGUELIKE_PROJECT_API FEnemyDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Data")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Data")
	float BaseHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Data")
	float BaseDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Data")
	float BaseRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Data")
	float BaseFireRate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Data")
	bool FollowsPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Data")
	TSubclassOf<ABaseEnemy> EnemyBP;
};