#pragma once


#include "BTTask_FireSpecialAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_FireSpecialAttack::UBTTask_FireSpecialAttack()
{
	NodeName = TEXT("Fire Special Attack");
}

// Called when node is reached. Runs for only once per execution.
EBTNodeResult::Type UBTTask_FireSpecialAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	m_ReachedTarget = false;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<uint8>(NextState));
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Calls special projectile fire function
	if (m_Enemy = Cast<ABaseBoss>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		m_Enemy->ShootSpecial();
		m_Enemy->OnSetFeedback.Broadcast(false);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	
}