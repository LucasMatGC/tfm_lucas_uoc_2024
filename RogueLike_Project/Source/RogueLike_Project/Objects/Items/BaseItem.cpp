// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

#include "GameFramework/GameModeBase.h"
#include "RogueLike_Project/RogueLike_ProjectCharacter.h"

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	RootCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(RootCollider);
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootCollider);
	
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	
	Super::BeginPlay();

	RootCollider->OnComponentBeginOverlap.AddDynamic(InteractComponent, &UInteractComponent::IsInteractable);
	RootCollider->OnComponentEndOverlap.AddDynamic(InteractComponent, &UInteractComponent::IsNoLongerInteractable);
	InteractComponent->OnInteract.AddDynamic(this, &ABaseItem::ABaseItem::PickUpItem);

	InteractComponent->InteractableName = ItemName;
	
}

// Called when the game ends or when despawned
void ABaseItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	RootCollider->OnComponentBeginOverlap.RemoveDynamic(InteractComponent, &UInteractComponent::IsInteractable);
	RootCollider->OnComponentEndOverlap.RemoveDynamic(InteractComponent, &UInteractComponent::IsNoLongerInteractable);
	InteractComponent->OnInteract.RemoveDynamic(this, &ABaseItem::ABaseItem::PickUpItem);
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called when player interacts with the item
void ABaseItem::PickUpItem(ACharacter* interactor)
{

	// If consumable, use immediately
	if (ItemType == EItemType::Consumable)
	{

		if (ARogueLike_ProjectCharacter* player = Cast<ARogueLike_ProjectCharacter>(interactor))
		{

			if (ConsumableHealth > 0)
			{
				
				player->HealPlayer(ConsumableHealth);
				
			}

			if (ConsumableAmmo > 0)
			{
				
				player->AddAmmo(ConsumableAmmo);
				
			}
			
		}
		
	}
	// If upgrade, show upgrade UI to select
	else
	{

		if (ARogueLike_ProjectCharacter* player = Cast<ARogueLike_ProjectCharacter>(interactor))
		{

			player->InventoryComponent->PickUpItem(this);
			
		}
		
	}

	//Hide item in world
	HideItem();
}

// Hide item in world
void ABaseItem::HideItem()
{

	// Hides visible components
	SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);

	// If item is consumbale, destroy it
	if (ItemType == EItemType::Consumable)
	{

		Destroy();
		
	}
	
}
