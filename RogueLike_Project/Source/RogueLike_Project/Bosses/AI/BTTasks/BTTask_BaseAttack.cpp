#pragma once


#include "BTTask_BaseAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_BaseAttack::UBTTask_BaseAttack()
{
	NodeName = TEXT("Base Attack");
}

// Called when node is reached. Runs for only once per execution.
EBTNodeResult::Type UBTTask_BaseAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	m_ReachedTarget = false;
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Set focus on player
	if (m_Enemy = Cast<ABaseBoss>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", AimTime);
		m_PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		OwnerComp.GetAIOwner()->SetFocus(m_PlayerPawn);
		bNotifyTick = true;
		return EBTNodeResult::InProgress;

	}

	return EBTNodeResult::Failed;
	
}

// Called every frame
void UBTTask_BaseAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// Reduce Aim time
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") - DeltaSeconds);

	m_Enemy->OnSetFeedback.Broadcast(true);
	
	// If aim time is reached, stop movement and fire base attack
	if (OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") <= 0)
	{
		
		OwnerComp.GetAIOwner()->StopMovement();
		m_Enemy->OnSetFeedback.Broadcast(false);
		m_Enemy->Fire(1);

		// Adds one to the current base projectile fires
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey()) + 1);

		// If the base projectiles to fire limit is reached, change enemy state, else, close node and wait for next fire
		if (OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey()) >= OwnerComp.GetBlackboardComponent()->GetValueAsInt("ProjectilesToFire"))
		{
		
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("NextState"), static_cast<uint8>(NextState));
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 0);
			
		}
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
	
	}

	
}
