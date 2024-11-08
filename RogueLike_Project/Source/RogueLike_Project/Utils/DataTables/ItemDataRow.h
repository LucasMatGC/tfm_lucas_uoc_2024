#pragma once

#include "CoreMinimal.h"
#include "RogueLike_Project/Objects/Items/BaseItem.h"
#include "ItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct ROGUELIKE_PROJECT_API FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	TSubclassOf<ABaseItem> ItemBP;
};