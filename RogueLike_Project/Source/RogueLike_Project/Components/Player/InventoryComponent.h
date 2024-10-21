// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueLike_Project/Items/Weapons/BaseWeapon.h"
#include "InventoryComponent.generated.h"

UCLASS()
class ROGUELIKE_PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	FTransform WeaponTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TArray<TSubclassOf<ABaseWeapon>> WeaponTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<ABaseWeapon*> Weapons;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ChangeWeapon(bool MoveForward);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void FireCurrentWeapon();

protected:

	virtual void BeginPlay() override;
	
private:

	int m_CurrentWeapon = 0;

};
