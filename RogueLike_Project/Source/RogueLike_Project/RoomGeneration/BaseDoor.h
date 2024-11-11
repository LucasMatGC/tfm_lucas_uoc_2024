// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDoor.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseDoor : public AActor
{
	GENERATED_BODY()

	
public:	
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	USceneComponent* RootScene;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* DoorMesh;

public:
	
	// Sets default values for this actor's properties
	ABaseDoor();
	

	UFUNCTION(BlueprintCallable)
	void OpenDoor();

	UFUNCTION(BlueprintCallable)
	void CloseDoor();

	UFUNCTION(BlueprintCallable)
	bool IsLocked();
	
	UFUNCTION(BlueprintCallable)
	void SetLocked(bool IsLocked);

private:

	bool m_bIsLocked = false;

};
