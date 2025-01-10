#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RogueLike_Project/Enemies/EnemyState.h"
#include "RogueLike_Project/Bosses/BaseBoss.h"
#include "BTTask_SpecialAttack.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API UBTTask_SpecialAttack : public UBTTask_BlackboardBase
{

	GENERATED_BODY()
	
public:

	// Set time enemy needs to aim at the player before shooting 
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", UIMin = "0.1"))
	float AimTime = 0.2;

public:
	
	UBTTask_SpecialAttack();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY(Transient)
	TObjectPtr<ABaseBoss> m_Enemy;

	UPROPERTY(Transient)
	TObjectPtr<APawn> m_PlayerPawn;

	bool m_ReachedTarget;
	
};
