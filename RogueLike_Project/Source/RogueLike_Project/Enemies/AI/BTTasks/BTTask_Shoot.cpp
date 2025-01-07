#pragma once


#include "BTTask_Shoot.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	m_ReachedTarget = false;
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (m_Enemy = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", AimTime);
		m_PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		OwnerComp.GetAIOwner()->SetFocus(m_PlayerPawn);
		OwnerComp.GetAIOwner()->MoveToActor(m_PlayerPawn);
		bNotifyTick = true;
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_Shoot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if ( (OwnerComp.GetAIOwner()->LineOfSightTo(m_PlayerPawn) &&
			FVector3d::Dist(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(),
				m_PlayerPawn->GetActorLocation()) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey())) ||
				m_ReachedTarget)
	{

		m_ReachedTarget = true;
		m_Enemy->OnSetFeedback.Broadcast(true);
		
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") - DeltaSeconds);


		if (OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") <= 0)
		{
			
			OwnerComp.GetAIOwner()->StopMovement();
			m_Enemy->Fire();
			m_Enemy->OnSetFeedback.Broadcast(false);
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("NextState"), static_cast<uint8>(NextState));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
		}
	}
	else
	{
		OwnerComp.GetAIOwner()->MoveToActor(m_PlayerPawn);
	}

	
}
