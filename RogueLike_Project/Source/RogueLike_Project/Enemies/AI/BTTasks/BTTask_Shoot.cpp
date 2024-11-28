#pragma once


#include "BTTask_Shoot.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

//TODO: Cambiar la logica del Shoot para que primero se asegure de tener linea de objetivo al player
EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (m_Enemy = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		
		m_PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		OwnerComp.GetAIOwner()->SetFocus(m_PlayerPawn);

		if ( OwnerComp.GetAIOwner()->LineOfSightTo(m_PlayerPawn) &&
			FVector3d::Dist(m_Enemy->GetActorLocation(),
				m_PlayerPawn->GetActorLocation()) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey()))
		{
			m_Enemy->Fire();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("NextState"), static_cast<uint8>(NextState));
			return EBTNodeResult::Succeeded;
		}
		else
		{
			bNotifyTick = true;
			OwnerComp.GetAIOwner()->MoveToActor(m_PlayerPawn);
			return EBTNodeResult::InProgress;
		}
		
	}


	return EBTNodeResult::Failed;
	
}

void UBTTask_Shoot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if ( OwnerComp.GetAIOwner()->LineOfSightTo(m_PlayerPawn) &&
			FVector3d::Dist(m_Enemy->GetActorLocation(),
				m_PlayerPawn->GetActorLocation()) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey()))
	{
		OwnerComp.GetAIOwner()->StopMovement();
		m_Enemy->Fire();
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("NextState"), static_cast<uint8>(NextState));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}