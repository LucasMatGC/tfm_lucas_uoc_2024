#pragma once


#include "BTTask_RandomizeNextAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RogueLike_Project/Bosses/BaseBoss.h"


UBTTask_RandomizeNextAttack::UBTTask_RandomizeNextAttack()
{
	NodeName = TEXT("Randomize Next Attack");
}

EBTNodeResult::Type UBTTask_RandomizeNextAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	float newRandomAttack = FMath::RandRange(0.0f, 1.0f);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), newRandomAttack);
	
	return EBTNodeResult::Succeeded;
	
}