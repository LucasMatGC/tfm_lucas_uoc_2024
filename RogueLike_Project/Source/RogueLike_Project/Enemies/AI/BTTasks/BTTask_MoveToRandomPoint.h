#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RogueLike_Project/Enemies/EnemyState.h"
#include "BTTask_MoveToRandomPoint.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API UBTTask_MoveToRandomPoint : public UBTTask_BlackboardBase
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxPatrolTime;

	UPROPERTY(EditAnywhere)
	UEnemyState NextState;

	UBTTask_MoveToRandomPoint();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float CurrentPatrolTime;
	
};
