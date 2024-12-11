


#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogueLike_Project/Bosses/BaseBoss.h"
#include "RogueLike_Project/Enemies/EnemyState.h"


void ABossAIController::BeginPlay()
{

	Super::BeginPlay();
	
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsFloat(TEXT("RandomPatrolRadius"), 1500.0f);
		GetBlackboardComponent()->SetValueAsEnum(TEXT("EnemyState"), static_cast<uint8>(UEnemyState::Attacking));

		if (ABaseBoss* Enemy = Cast<ABaseBoss>(GetOwner()))
		{
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyFireRange"), Enemy->Range);
			GetBlackboardComponent()->SetValueAsInt(TEXT("ProjectilesToFire"), Enemy->ProjectilesToFire);
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyMeleeRange"), Enemy->MeleeRange);
		}
		else
		{
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyRange"), 1000.0f);
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyMeleeRange"), 100.f);
		}
		
	}
	
}

void ABossAIController::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
}
