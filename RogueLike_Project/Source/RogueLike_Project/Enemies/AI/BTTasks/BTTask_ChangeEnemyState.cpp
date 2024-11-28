#pragma once


#include "BTTask_ChangeEnemyState.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "RogueLike_Project/Enemies/EnemyState.h"

UBTTask_ChangeEnemyState::UBTTask_ChangeEnemyState()
{
	NodeName = TEXT("Change Enemy State");
}

EBTNodeResult::Type UBTTask_ChangeEnemyState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(
		GetSelectedBlackboardKey(),
		OwnerComp.GetBlackboardComponent()->GetValueAsEnum("NextState"));

	return EBTNodeResult::Succeeded;
	
}