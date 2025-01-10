#pragma once


#include "BTTask_FireMeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_FireMeleeAttack::UBTTask_FireMeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

// Called when node is reached. Runs for only once per execution.
EBTNodeResult::Type UBTTask_FireMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	m_ReachedTarget = false;
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Set focus on player and start moving towards it
	if (m_Enemy = Cast<ABaseBoss>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", AimTime);
		m_PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		OwnerComp.GetAIOwner()->SetFocus(m_PlayerPawn);
		OwnerComp.GetAIOwner()->MoveToActor(m_PlayerPawn);
		bNotifyTick = true;
		m_Enemy->OnSetFeedback.Broadcast(true);
		return EBTNodeResult::InProgress;
	
	}
	
	return EBTNodeResult::Failed;
}

// Called every frame
void UBTTask_FireMeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// If player is in attack range of enemy or if already reach him
	if ( (OwnerComp.GetAIOwner()->LineOfSightTo(m_PlayerPawn) &&
			FVector3d::Dist(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(),
				m_PlayerPawn->GetActorLocation()) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey())) || m_ReachedTarget)
	{

		m_ReachedTarget = true;
		
		// Reduce Aim time
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") - DeltaSeconds);

		// If aim time is reached, stop movement and fire melee attack
		if (OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") <= 0)
		{
			
			OwnerComp.GetAIOwner()->StopMovement();
			m_Enemy->Fire(2);
			m_Enemy->OnSetFeedback.Broadcast(false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
		}
	}
	// Else, keep moving towards player
	else
	{
		OwnerComp.GetAIOwner()->MoveToActor(m_PlayerPawn);
	}

	
}
