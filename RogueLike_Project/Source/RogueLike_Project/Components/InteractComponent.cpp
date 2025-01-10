// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "RogueLike_Project/RogueLike_ProjectCharacter.h"


// Sets default values
UInteractComponent::UInteractComponent()
{
	
}

// Called when the game starts or when spawned
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called when the game ends or when despawned
void UInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
		
	Super::EndPlay(EndPlayReason);
		
}

// Called when overlaps with another actor
void UInteractComponent::IsInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// If other actor is player, add interactive to player
	if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(OtherActor))
	{

		Character->AddInteractable(this);
		
	}
	
}

// Called when no longer overlaps with another actor
void UInteractComponent::IsNoLongerInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// If other actor is player, remove interactive to player
	if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(OtherActor))
	{

		Character->RemoveInteractable(this);
		
	}
	
}

// Call interaction function of actor
void UInteractComponent::Interact(ACharacter* interactor)
{

	OnInteract.Broadcast(interactor);
	
}
