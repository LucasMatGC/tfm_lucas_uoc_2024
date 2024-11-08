// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "BaseItem.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* RootCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;

public:
	
	// Sets default values for this actor's properties
	ABaseItem();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPickUpItem);
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FPickUpItem OnPickUpItem;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);

	virtual void Tick(float DeltaTime) override;
};
