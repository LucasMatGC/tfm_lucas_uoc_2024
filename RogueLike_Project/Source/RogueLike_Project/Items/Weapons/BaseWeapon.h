// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* RootScene;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Variables")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Variables")
	float FireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Variables")
	float Range;

	UFUNCTION(BlueprintCallable)
	virtual void Fire();
};
