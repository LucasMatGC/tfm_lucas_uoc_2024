// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueLike_Project/Objects/Weapons/BaseWeapon.h"
#include "InventoryComponent.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class ROGUELIKE_PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Configuration")
	FTransform WeaponTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Weapons")
	USceneComponent* FirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Configuration")
	TArray<TSubclassOf<ABaseWeapon>> WeaponTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Weapons")
	TArray<ABaseWeapon*> Weapons;
	
	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Upgrades")
	TArray<TObjectPtr<ABaseItem>> CommonUpgrades;

public:
	
	UInventoryComponent();
	
	// Sets default values for this actor's properties
	UFUNCTION(Blueprintable)
	void SetupInventory(USceneComponent* newFirePoint);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Weapons")
	void ChangeWeapon(bool MoveForward);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Weapons")
	void FireCurrentWeapon();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Weapons", BlueprintPure)
	ABaseWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Weapons")
	void AddAmmo(float ConsumableAmmo);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Weapons")
	void PickUpItem(ABaseItem* NewPickedUpItem);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Weapons")
	void AttachPickedUpItem(int indexOfWeapon);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeSelection, ABaseItem*, pickedUpgrade, bool, bShowUpgradeHUD);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Upgrades")
	FOnUpgradeSelection OnUpgradeSelection;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeMaxHealth, float, healthUpgrade);
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Upgrades")
	FOnUpgradeMaxHealth OnUpgradeMaxHealth;
	
protected:

	virtual void BeginPlay() override;
	
private:

	int m_CurrentWeapon = 0;

	UPROPERTY(Transient)
	ABaseItem* PickedUpItem;

};
