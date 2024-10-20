// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "MeeleWeapon.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API AMeeleWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeeleWeapon();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Fire() override;

};
