// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractComponent.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class ROGUELIKE_PROJECT_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UInteractComponent();

	UFUNCTION()
	void IsInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void IsNoLongerInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Interact();

	/*
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInteractable, UInteractComponent*, newInteractable);
	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FUpdateInteractable OnUpdateInteractable;
	*/
	
protected:
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration|Health Values", meta = (ClampMin = "0", UIMin = "0"))
	float MaxHealth = 200.0f;
	
};
