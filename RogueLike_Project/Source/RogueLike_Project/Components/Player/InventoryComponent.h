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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Configuration")
	FTransform WeaponTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Configuration")
	TArray<TSubclassOf<ABaseWeapon>> WeaponTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Weapons")
	TArray<ABaseWeapon*> Weapons;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Weapons")
	void ChangeWeapon(bool MoveForward);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Weapons")
	void FireCurrentWeapon();

protected:

	virtual void BeginPlay() override;
	
private:

	int m_CurrentWeapon = 0;

};
