#pragma once

#include "CoreMinimal.h"
#include "RogueLike_Project/Bosses/BaseBoss.h"
#include "BossDataRow.generated.h"

USTRUCT(BlueprintType)
struct ROGUELIKE_PROJECT_API FBossDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss Data")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss Data")
	TSubclassOf<ABaseBoss> BossBP;
};