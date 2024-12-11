#pragma once


#include "BTTask_FireSpecialAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_FireSpecialAttack::UBTTask_FireSpecialAttack()
{
	NodeName = TEXT("Shoot Special Attack");
}

EBTNodeResult::Type UBTTask_FireSpecialAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	m_ReachedTarget = false;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("NextState"), static_cast<uint8>(NextState));
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	if (m_Enemy = Cast<ABaseBoss>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		m_Enemy->ShootSpecial();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	
}