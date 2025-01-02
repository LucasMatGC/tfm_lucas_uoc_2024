// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRoom.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> RootScene = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TArray<USceneComponent*> ExitsLists;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TSubclassOf<ABaseRoom> StartRoomType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TSubclassOf<ABaseRoom> ItemRoomType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TSubclassOf<ABaseRoom> BossRoomType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TArray<TSubclassOf<ABaseRoom>> RoomTypeList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	int MaxRoom = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TArray<TObjectPtr<ABaseRoom>> RoomList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TObjectPtr<ABaseRoom> SpawnRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TObjectPtr<ABaseRoom> BossRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TObjectPtr<ABaseRoom> ItemRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation|GenerateVariables")
	TSubclassOf<AActor> DoorBP;

public:
	
	UFUNCTION(BlueprintCallable)
	void Initialize(int maxRooms, AGameplayGameMode* GameMode);
	
	UFUNCTION(BlueprintCallable)
	void GenerateRooms();
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SpawnNextRoom();
	
	virtual void AddExits();

	virtual bool IsOverlaping();
	
	void SpawnBossRoom();
	
	void SpawnItemRoom();

	virtual void CloseRemainingExits();

	virtual TSubclassOf<ABaseRoom> GetRandomRoomType();
	
	virtual USceneComponent* GetRandomExit();

private:

	UPROPERTY(Transient)
	TObjectPtr<ABaseRoom> m_LatestRoom;
	
	UPROPERTY(Transient)
	TObjectPtr<AGameplayGameMode> m_GameMode;

	UPROPERTY(Transient)
	int CurrentRooms = 0;

};
