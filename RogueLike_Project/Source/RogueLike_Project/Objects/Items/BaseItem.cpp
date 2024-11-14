// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

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

void ABaseItem::PickUpItem()
{

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, "Item Picked Up!!!!!!");

	HideItem();
	
}

void ABaseItem::ApplyUpgrade(ABaseProjectile* projectile)
{
	switch (ItemType)
	{

		case EItemType::DamageTypeUpgrade:

			projectile->DamageType = DamageType;
			break;
		
		case EItemType::ModifierUpgrade:

			projectile->ProjectileMesh->SetStaticMesh(NewProjectileMesh);
			//TODO: Modify Collision profile
			//TODO: Modify Trajectory of Projectile
			break;
		
	}
	
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	
	Super::BeginPlay();

	RootCollider->OnComponentBeginOverlap.AddDynamic(InteractComponent, &UInteractComponent::IsInteractable);
	RootCollider->OnComponentEndOverlap.AddDynamic(InteractComponent, &UInteractComponent::IsNoLongerInteractable);
	
}

void ABaseItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	RootCollider->OnComponentBeginOverlap.RemoveDynamic(InteractComponent, &UInteractComponent::IsInteractable);
	RootCollider->OnComponentEndOverlap.RemoveDynamic(InteractComponent, &UInteractComponent::IsNoLongerInteractable);
	
	Super::EndPlay(EndPlayReason);
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseItem::HideItem()
{

	// Hides visible components
	SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);
	
}
