﻿#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RogueLike_Project/Enemies/EnemyState.h"
#include "RogueLike_Project/Bosses/BaseBoss.h"
#include "BTTask_FireMeleeAttack.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API UBTTask_FireMeleeAttack : public UBTTask_BlackboardBase
{

	GENERATED_BODY()
	
public:
	
	UBTTask_FireMeleeAttack();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY(Transient)
	TObjectPtr<ABaseBoss> m_Enemy;

	UPROPERTY(Transient)
	TObjectPtr<APawn> m_PlayerPawn;

	bool m_ReachedTarget;
	
};