#pragma once


#include "BTTask_HideMeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RogueLike_Project/Bosses/BaseBoss.h"


UBTTask_HideMeleeAttack::UBTTask_HideMeleeAttack()
{
	NodeName = TEXT("Hide Melee Attack");
}

// Called when node is reached. Runs for only once per execution.
EBTNodeResult::Type UBTTask_HideMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<uint8>(NextState));
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Calls deactivate melee collider function
	if (ABaseBoss* Boss = Cast<ABaseBoss>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Boss->UseMeleeCollider(false);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	
}