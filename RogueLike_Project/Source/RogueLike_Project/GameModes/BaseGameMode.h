// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RogueLike_Project/RoomGeneration/MapGenerator.h"
#include "BaseGameMode.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	int PredefinedSeed = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	int MinRooms = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	int MaxRooms = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Configuration")
	TSubclassOf<AMapGenerator> MapGeneratorType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	FRandomStream RandomStream;
	
	UPROPERTY(Transient)
	TObjectPtr<AMapGenerator> m_MapGenerator;

	UPROPERTY(Transient)
	TObjectPtr<APlayerController> m_Player;
	
public:	
	// Sets default values for this actor's properties
	ABaseGameMode();
	
	UFUNCTION(BlueprintCallable)
	int RandomRangeInt(int Min, int Max);
	
	UFUNCTION(BlueprintCallable)
	float RandomRangeFloat(float Min, float Max);

	
protected:

	virtual void BeginPlay() override;
	void SetSeed();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	int CurrentRooms;

private:

	bool m_IsGamePaused = false;	
	
};
