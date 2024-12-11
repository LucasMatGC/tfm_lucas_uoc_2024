#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RogueLike_Project/Enemies/EnemyState.h"
#include "RogueLike_Project/Bosses/BaseBoss.h"
#include "BTTask_FireSpecialAttack.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API UBTTask_FireSpecialAttack : public UBTTask_BlackboardBase
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	UEnemyState NextState;
	
	UBTTask_FireSpecialAttack();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(Transient)
	TObjectPtr<ABaseBoss> m_Enemy;

	UPROPERTY(Transient)
	TObjectPtr<APawn> m_PlayerPawn;

	bool m_ReachedTarget;
	
};
