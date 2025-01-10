#pragma once


#include "BTTask_MoveToRandomPoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"


UBTTask_MoveToRandomPoint::UBTTask_MoveToRandomPoint()
{
	NodeName = TEXT("Move To Random Point");
}

// Called when node is reached. Runs for only once per execution.
EBTNodeResult::Type UBTTask_MoveToRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// If AI has no owner, return
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	
	// If owner has no pawn, return
	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	// If there's no navigational system to move, return
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

	// If no navigational point is generated, return
	if (!bSuccess)
	{
		return EBTNodeResult::Failed;
	}

	// Move to selected poing
	OwnerComp.GetAIOwner()->MoveToLocation(NewPosition, 100);

	CurrentPatrolTime = MaxPatrolTime;

	bNotifyTick = true;
	
	return EBTNodeResult::InProgress;
	
}

// Called every frame
void UBTTask_MoveToRandomPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	CurrentPatrolTime -= DeltaSeconds;
	
	// If patrol point is still not reached and there's patrol time left, return and retry
	if (OwnerComp.GetAIOwner()->GetMoveStatus() == EPathFollowingStatus::Type::Moving && CurrentPatrolTime > 0.0f)
	{

		return;
		
	}

	// If reached or patrol time is reached, finish task
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("NextState"), static_cast<uint8>(NextState));
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
