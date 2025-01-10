


#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogueLike_Project/Enemies/BaseEnemy.h"
#include "RogueLike_Project/Enemies/EnemyState.h"


// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{

	Super::BeginPlay();

	// Setup Behaviour Tree variables
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);


		GetBlackboardComponent()->SetValueAsBool(TEXT("TargetReached"), false);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsFloat(TEXT("RandomPatrolRadius"), 1500.0f);
		GetBlackboardComponent()->SetValueAsEnum(TEXT("EnemyState"), static_cast<uint8>(UEnemyState::Attacking));

		if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GetOwner()))
		{
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyRange"), Enemy->Range);	
		}
		else
		{
			GetBlackboardComponent()->SetValueAsFloat(TEXT("EnemyRange"), 1000.0f);
		}
		
	}
	
}

// Called every frame
void AEnemyAIController::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	// Update player position
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
}
