// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RogueLike_Project/RoomGeneration/MapGenerator.h"
#include "RogueLike_Project/Utils/GameInstance/BaseGameInstance.h"
#include "GameplayGameMode.generated.h"

USTRUCT(BlueprintType)
struct FLevelVariables
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	int MinRooms = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	int MaxRooms = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	int CurrentLevel = 1;
	
};

UCLASS()
class ROGUELIKE_PROJECT_API AGameplayGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Configuration")
	TSubclassOf<AMapGenerator> MapGeneratorType;
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AMapGenerator> MapGenerator;

	UPROPERTY(Transient, BlueprintReadWrite)
	TObjectPtr<APlayerController> Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Configuration")
	TMap<FName, FLevelVariables> LevelConfigurationMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Configuration")
	FLevelVariables CurrentLevelConfiguration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Configuration")
	float CurrentGameTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Configuration")
	float ConsumableSpawnThreshold = 0.3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Configuration")
	float UpgradeSpawnThreshold = 0.9;

	UPROPERTY(EditDefaultsOnly, meta=(RowType = "ItemDataRow"), Category = "Configuration")
	TSoftObjectPtr<UDataTable> ConsumableTable;

	UPROPERTY(EditDefaultsOnly, meta=(RowType = "ItemDataRow"), Category = "Configuration")
	TSoftObjectPtr<UDataTable> UpgradeTable;
	
public:	
	// Sets default values for this actor's properties
	AGameplayGameMode();

	UFUNCTION(BlueprintCallable)
	void LoadLevelData();

	UFUNCTION(BlueprintCallable)
	void LoadNextLevel();
	
	UFUNCTION(BlueprintCallable)
	int RandomRangeInt(int Min, int Max);
	
	UFUNCTION(BlueprintCallable)
	float RandomRangeFloat(float Min, float Max);
	
	UFUNCTION(BlueprintCallable)
	void PreparePlayer();
	
	UFUNCTION(BlueprintCallable)
	void GameOver();

	UFUNCTION(BlueprintCallable)
	void CheckItemSpawn(float RandomizedItemSpawnRate, FTransform SpawnTransform, bool isMeleeDamage);

protected:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	void SetSeed();


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	int CurrentRooms;

private:

	bool m_IsGamePaused = false;

	bool m_IsPlayerPrepared = false;

	UPROPERTY(Transient)
	TObjectPtr<UBaseGameInstance> m_GameInstance;
	
	FGameVariables m_GameVariables;
	
};
