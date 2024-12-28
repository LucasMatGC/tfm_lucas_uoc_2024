#pragma once


#include "BTTask_BaseAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_BaseAttack::UBTTask_BaseAttack()
{
	NodeName = TEXT("Base Attack");
}

EBTNodeResult::Type UBTTask_BaseAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	m_ReachedTarget = false;
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", AimTime);
	m_PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	OwnerComp.GetAIOwner()->SetFocus(m_PlayerPawn);
	bNotifyTick = true;
	return EBTNodeResult::InProgress;
	
}

void UBTTask_BaseAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") - DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") <= 0)
	{
		
		OwnerComp.GetAIOwner()->StopMovement();
		if (m_Enemy = Cast<ABaseBoss>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			m_Enemy->Fire(1);
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey()) + 1);
		
		if (OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey()) >= OwnerComp.GetBlackboardComponent()->GetValueAsInt("ProjectilesToFire"))
		{
		
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("NextState"), static_cast<uint8>(NextState));
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 0);
			
		}
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
	
	}

	
}
