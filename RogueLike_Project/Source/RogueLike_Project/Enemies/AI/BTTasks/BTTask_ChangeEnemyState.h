#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RogueLike_Project/Enemies/BaseEnemy.h"
#include "BTTask_ChangeEnemyState.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API UBTTask_ChangeEnemyState : public UBTTask_BlackboardBase
{

	GENERATED_BODY()
	
public:
	
	UBTTask_ChangeEnemyState();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
