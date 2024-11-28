#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API AEnemyAIController : public AAIController
{

	GENERATED_BODY()
	
public:
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Configuration|AI")
	class UBehaviorTree* AIBehavior;
	
};
