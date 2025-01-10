


#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogueLike_Project/Bosses/BaseBoss.h"
#include "RogueLike_Project/Enemies/EnemyState.h"


// Called when the game starts or when spawned
void ABossAIController::BeginPlay()
{

	Super::BeginPlay();

	// Setup Behaviour tree base variables
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsFloat(TEXT("RandomPatrolRadius"), 800.0f);
		GetBlackboardComponent()->SetValueAsEnum(TEXT("EnemyState"), static_cast<uint8>(UEnemyState::Patrolling));
		GetBlackboardComponent()->SetValueAsFloat(TEXT("NextAttackPercentage"), 1.0f);
		GetBlackboardComponent()->SetValueAsInt(TEXT("CurrentProjectile"), 0);

		if (ABaseBoss* Enemy = Cast<ABaseBoss>(GetOwner()))
		{
			GetBlackboardComponent()->SetValueAsFloat(TEXT("SpecialProjectileWaitTime"), Enemy->SpecialProjectileWaitTime);
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyFireRange"), Enemy->Range);
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyMeleeRange"), Enemy->MeleeRange);
			GetBlackboardComponent()->SetValueAsInt(TEXT("ProjectilesToFire"), Enemy->ProjectilesToFire);
		}
		else
		{
			GetBlackboardComponent()->SetValueAsFloat(TEXT("SpecialProjectileWaitTime"), 2.0);
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyFireRange"), 1200.0f);
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyMeleeRange"), 275.f);
			GetBlackboardComponent()->SetValueAsInt(TEXT("ProjectilesToFire"), 10);
		}
		
	}
	
}

// Called every frame
void ABossAIController::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
}
