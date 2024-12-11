#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RogueLike_Project/Enemies/EnemyState.h"
#include "BTTask_HideMeleeAttack.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API UBTTask_HideMeleeAttack : public UBTTask_BlackboardBase
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	UEnemyState NextState;
	
	UBTTask_HideMeleeAttack();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
