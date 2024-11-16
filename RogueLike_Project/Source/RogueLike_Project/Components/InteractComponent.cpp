// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "RogueLike_Project/RogueLike_ProjectCharacter.h"


// Sets default values
UInteractComponent::UInteractComponent()
{
	
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
		
	Super::EndPlay(EndPlayReason);
		
}

void UInteractComponent::IsInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(OtherActor))
	{

		Character->AddInteractable(this);
		
	}
	
}

void UInteractComponent::IsNoLongerInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if (ARogueLike_ProjectCharacter* Character = Cast<ARogueLike_ProjectCharacter>(OtherActor))
	{

		Character->RemoveInteractable(this);
		
	}
	
}

void UInteractComponent::Interact(ACharacter* interactor)
{

	//TODO: Actual interaction (call owner function 'Interact')
	OnInteract.Broadcast(interactor);
	
}
