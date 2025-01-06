// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "RogueLike_Project/Components/InteractComponent.h"
#include "RogueLike_Project/Objects/Weapons/BaseWeapon.h"
#include "Upgrades/UpgradeStruct.h"
#include "BaseItem.generated.h"

/* Different types of items
 *	Consumable:		Items that are instantly use to refill the player's health or ammunition
 *	Upgrade:		Item that applies an upgrade to the player or a weapon
 */
UENUM(BlueprintType)
enum class EItemType
{
	Consumable = 0,
	Upgrade = 1
};

UCLASS()
class ROGUELIKE_PROJECT_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* RootCollider;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;
	
	/** Health component **/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UInteractComponent* InteractComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
	FName ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
	EItemType ItemType = EItemType::Consumable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
	float RotationSpeed = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::Consumable", EditConditionHides), Category = "Configuration|Consumable")
	float ConsumableHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::Consumable", EditConditionHides), Category = "Configuration|Consumable")
	float ConsumableAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::Upgrade", EditConditionHides), Category = "Configuration|Upgrade")
	FUpgradeStruct DefaultUpgrade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::Upgrade", EditConditionHides), Category = "Configuration|Upgrade")
	TMap<TSubclassOf<ABaseWeapon>, FUpgradeStruct> UpgradeMapping;

public:
	
	// Sets default values for this actor's properties
	ABaseItem();
	
	UFUNCTION(BlueprintCallable)
	void PickUpItem(ACharacter* interactor);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUseConsumable, float, health, float, ammo);
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnUseConsumable OnUseConsumable;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);

	virtual void Tick(float DeltaTime) override;

	virtual void HideItem();
};
