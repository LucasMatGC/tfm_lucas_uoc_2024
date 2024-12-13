﻿#pragma once


#include "BTTask_FireMeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_FireMeleeAttack::UBTTask_FireMeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_FireMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	m_ReachedTarget = false;
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	m_PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	OwnerComp.GetAIOwner()->SetFocus(m_PlayerPawn);
	OwnerComp.GetAIOwner()->MoveToActor(m_PlayerPawn);
	bNotifyTick = true;
	return EBTNodeResult::InProgress;
	
}

void UBTTask_FireMeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if ( OwnerComp.GetAIOwner()->LineOfSightTo(m_PlayerPawn) &&
			FVector3d::Dist(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(),
				m_PlayerPawn->GetActorLocation()) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey()))
	{

		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CurrentAimTime", OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") - DeltaSeconds);

		if (OwnerComp.GetBlackboardComponent()->GetValueAsFloat("CurrentAimTime") <= 0)
		{
			
			OwnerComp.GetAIOwner()->StopMovement();
			if (m_Enemy = Cast<ABaseBoss>(OwnerComp.GetAIOwner()->GetPawn()))
			{
				m_Enemy->Fire(2);
			}
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
		}
	}
	else
	{
		OwnerComp.GetAIOwner()->MoveToActor(m_PlayerPawn);
	}

	
}
