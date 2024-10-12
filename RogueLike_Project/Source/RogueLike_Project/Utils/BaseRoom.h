// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "BaseRoom.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseRoom();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	UArrowComponent* Direction;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
