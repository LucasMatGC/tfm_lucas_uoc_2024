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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenerateVariables")
	TArray<USceneComponent*> ExitsLists;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenerateVariables")
	TSubclassOf<ABaseRoom> StartRoomType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenerateVariables")
	TArray<TSubclassOf<ABaseRoom>> RoomTypeList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenerateVariables")
	int MaxRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenerateVariables")
	TArray<TObjectPtr<ABaseRoom>> RoomList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenerateVariables")
	TSubclassOf<AActor> DoorBP;

	UFUNCTION(BlueprintCallable)
	void GenerateRooms();
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SpawnNextRoom();
	
	virtual void AddExits();

	virtual bool IsOverlaping();

	virtual void CloseRemainingExits();

	virtual TSubclassOf<ABaseRoom> GetRandomRoomType();
	
	virtual USceneComponent* GetRandomExit();

private:

	UPROPERTY(Transient)
	TObjectPtr<ABaseRoom> m_LatestRoom;

	UPROPERTY(Transient)
	int CurrentRooms = 0;

};
