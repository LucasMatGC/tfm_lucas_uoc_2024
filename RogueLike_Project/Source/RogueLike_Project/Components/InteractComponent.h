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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration|Interactable")
	FName InteractableName;
	
public:	
	// Sets default values for this actor's properties
	UInteractComponent();

	UFUNCTION()
	void IsInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void IsNoLongerInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Interact(ACharacter* interactor);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, ACharacter*, interactor);
	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FOnInteract OnInteract;
	
protected:
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
