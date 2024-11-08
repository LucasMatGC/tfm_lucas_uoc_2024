// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "RogueLike_Project/Enemies/BaseEnemy.h"
#include "RogueLike_Project/Utils/DataTables/EnemyDataRow.h"
#include "RogueLike_Project/Utils/DataTables/ItemDataRow.h"
#include "BaseRoom.generated.h"

UENUM(BlueprintType)
enum class ERoomFunctionality
{
	RandomEnemies = 0,
	RandomItems = 1,
	FullyRandom = 2,
	Customized = 3,
	Empty = 4
};

UCLASS()
class ROGUELIKE_PROJECT_API ABaseRoom : public AActor
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* RootScene;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* ExitsFolder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* GeometryFolder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* OverlapFolder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SpawnPointFolder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* ItemsFolder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* TriggersFolder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UArrowComponent* Direction;

	UPROPERTY(EditDefaultsOnly, meta=(RowType=EnemyDataRow))
	FDataTableRowHandle EnemyRowHandle;

	UPROPERTY(EditDefaultsOnly, meta=(RowType=ItemDataRow))
	FDataTableRowHandle ItemRowHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ERoomFunctionality Functionality = ERoomFunctionality::RandomEnemies;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "Functionality == ERoomFunctionality::RandomEnemies || Functionality == ERoomFunctionality::FullyRandom",
			EditConditionHides))
	TArray<FText> EnemiesToSpawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "Functionality == ERoomFunctionality::RandomItems || Functionality == ERoomFunctionality::FullyRandom",
			EditConditionHides))
	TArray<FText> ItemsToSpawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "Functionality == ERoomFunctionality::Customized", EditConditionHides))
	TMap<USceneComponent*, FText> EnemiesPerSpawnPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "Functionality == ERoomFunctionality::Customized", EditConditionHides))
	TMap<USceneComponent*, FText> ItemsPerSpawnPoint;

	UPROPERTY(Transient)
	TArray<TObjectPtr<ABaseEnemy>> SpawnedEnemies;

	UPROPERTY(Transient)
	TArray<TObjectPtr<ABaseItem>> SpawnedItems;
	
public:	
	// Sets default values for this actor's properties
	ABaseRoom();

	UFUNCTION(BlueprintCallable)
	void SpawnEnemies();

	UFUNCTION(BlueprintCallable)
	void SpawnItems();
	
	UFUNCTION(BlueprintCallable)
	void SpawnCustom();
	
	UFUNCTION(BlueprintCallable)
	void OpenDoors();
	
	UFUNCTION(BlueprintCallable)
	void CloseDoors();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void PlayerEnters(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHasBeenVisited = false;


};
