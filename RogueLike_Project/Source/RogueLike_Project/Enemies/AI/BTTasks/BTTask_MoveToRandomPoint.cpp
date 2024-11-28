#pragma once


#include "BTTask_MoveToRandomPoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "RogueLike_Project/Enemies/EnemyState.h"


UBTTask_MoveToRandomPoint::UBTTask_MoveToRandomPoint()
{
	NodeName = TEXT("Move To Random Point");
}

EBTNodeResult::Type UBTTask_MoveToRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}
	
	FVector Origin = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	FVector NewPosition;
	
	bool bSuccess = NavSys->K2_GetRandomReachablePointInRadius(GetWorld(),
		Origin,
		NewPosition,
		OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey()));

	if (!bSuccess)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetAIOwner()->MoveToLocation(NewPosition, 100);

	CurrentPatrolTime = MaxPatrolTime;

	bNotifyTick = true;
	
	return EBTNodeResult::InProgress;
	
}

void UBTTask_MoveToRandomPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	CurrentPatrolTime -= DeltaSeconds;
	
	if (OwnerComp.GetAIOwner()->GetMoveStatus() == EPathFollowingStatus::Type::Moving && CurrentPatrolTime > 0.0f)
	{

		return;
		
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("NextState"), static_cast<uint8>(NextState));
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
