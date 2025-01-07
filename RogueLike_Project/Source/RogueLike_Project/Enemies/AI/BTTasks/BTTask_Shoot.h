#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RogueLike_Project/Enemies/BaseEnemy.h"
#include "RogueLike_Project/Enemies/EnemyState.h"
#include "BTTask_Shoot.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API UBTTask_Shoot : public UBTTask_BlackboardBase
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	UEnemyState NextState;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", UIMin = "0.1"))
	float AimTime = 0.5;
	
	UBTTask_Shoot();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY(Transient)
	TObjectPtr<ABaseEnemy> m_Enemy;

	UPROPERTY(Transient)
	TObjectPtr<APawn> m_PlayerPawn;

	bool m_ReachedTarget;
	
};
