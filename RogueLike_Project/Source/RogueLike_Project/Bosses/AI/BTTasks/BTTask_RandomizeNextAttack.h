#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RandomizeNextAttack.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API UBTTask_RandomizeNextAttack : public UBTTask_BlackboardBase
{

	GENERATED_BODY()
	
public:
	
	UBTTask_RandomizeNextAttack();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
