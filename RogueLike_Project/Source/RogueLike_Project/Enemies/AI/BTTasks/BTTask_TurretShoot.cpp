#pragma once


#include "BTTask_TurretShoot.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_TurretShoot::UBTTask_TurretShoot()
{
	NodeName = TEXT("Shoot");
}

// Called when node is reached. Runs for only once per execution.
EBTNodeResult::Type UBTTask_TurretShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

// Called every frame
void UBTTask_TurretShoot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// If player is in attack range of enemy
	if ( OwnerComp.GetAIOwner()->LineOfSightTo(m_PlayerPawn) &&
			FVector3d::Dist(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(),
				m_PlayerPawn->GetActorLocation()) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey()))
	{

		// Reduce Aim time
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") - DeltaSeconds);

		// If aim time is reached, fire
		if (OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") <= 0)
		{
			
			if (m_Enemy = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn()))
			{
				m_Enemy->Fire();
			}

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
		}
	}

	
}
