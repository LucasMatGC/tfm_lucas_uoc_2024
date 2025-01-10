// Copyright Epic Games, Inc. All Rights Reserved.

#include "RogueLike_ProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/Player/InventoryComponent.h"
#include "Components/HealthComponent.h"
#include "GameModes/GameplayGameMode.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ARogueLike_ProjectCharacter

ARogueLike_ProjectCharacter::ARogueLike_ProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 650.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Defines the Fire point of the weapons (Spawn point for projectiles)
	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(GetMesh());

	// Defines the Fire point of the weapons (Spawn point for projectiles)
	MeleeAttackMeshCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeAttackMeshCollider"));
	MeleeAttackMeshCollider->SetupAttachment(GetMesh());
	MeleeAttackMeshCollider->SetHiddenInGame(true);
	MeleeAttackMeshCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Add Inventory Component and define the weapon socket
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));	
	GetMesh()->SetCollisionProfileName("Pawn");

}

void ARogueLike_ProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Setup Inventory Component
	InventoryComponent->SetupInventory(FirePoint);

	HealthComponent->OnUpdateCurrentHealth.AddDynamic(this, &ARogueLike_ProjectCharacter::TakeDamage);
	HealthComponent->OnProcessDeath.AddDynamic(this, &ARogueLike_ProjectCharacter::KillPlayer);
	InventoryComponent->OnUpgradeMaxHealth.AddDynamic(HealthComponent, &UHealthComponent::UpgradeMaxHealth);
	MeleeAttackMeshCollider->OnComponentBeginOverlap.AddDynamic(this, &ARogueLike_ProjectCharacter::ApplyMeleeDamage);
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->bShowMouseCursor = true;
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	OnPlayerReady.Broadcast();
}

void ARogueLike_ProjectCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	HealthComponent->OnUpdateCurrentHealth.RemoveDynamic(this, &ARogueLike_ProjectCharacter::TakeDamage);
	HealthComponent->OnProcessDeath.RemoveDynamic(this, &ARogueLike_ProjectCharacter::KillPlayer);
	InventoryComponent->OnUpgradeMaxHealth.RemoveDynamic(HealthComponent, &UHealthComponent::UpgradeMaxHealth);
	MeleeAttackMeshCollider->OnComponentBeginOverlap.RemoveDynamic(this, &ARogueLike_ProjectCharacter::ApplyMeleeDamage);
	
	Super::EndPlay(EndPlayReason);
}

// Triggers every frame
void ARogueLike_ProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If player currently has invulnerability time, decrease it
	if (CurrentInvulnerabilityTime > 0.0f)
	{

		CurrentInvulnerabilityTime -= DeltaTime;

		if (CurrentInvulnerabilityTime <= 0.0f && m_IsPlayerAlive)
		{

			// Reactivate Damage input to health component
			HealthComponent->SetCanTakeDamage(true);
			
		}
		
	}
	
	if (m_IsPlayerAlive && m_CanPlayerMove)
	{

		// Calculate the rotation of the player
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{

			FVector2D mousePos = FVector2D::ZeroVector;
			FHitResult* hit = new FHitResult();
			FVector startTrace;
			FVector direction;
			FCollisionQueryParams* params = new FCollisionQueryParams();

			// Get position of cursor on screen
			PlayerController->GetMousePosition(mousePos.X, mousePos.Y);

			//Proyect cursor position from camera to scene to get point of view of player
			PlayerController->DeprojectScreenPositionToWorld(mousePos.X, mousePos.Y, startTrace, direction);

			FVector endTrace = direction * (CameraBoom->TargetArmLength * 30) + startTrace;

			if (GetWorld()->LineTraceSingleByChannel(*hit, startTrace, endTrace, ECollisionChannel::ECC_Camera, *params))
			{

				if (hit->GetActor() != nullptr)
				{

					//If Hit, rotate player to hit point
					SetActorRotation( FRotator(
						0,
						UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), hit->Location).Yaw,
						0));
					
				}
				
			}
			
		}
		
	}
	
}

// Apply damage taken to health component
void ARogueLike_ProjectCharacter::ApplyMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	InventoryComponent->ApplyMeleeDamage(OtherActor);
	
}

// Heal player
void ARogueLike_ProjectCharacter::HealPlayer(float ConsumableHealth)
{

	HealthComponent->Heal(ConsumableHealth);
	
}

// Add ammo to all weapons
void ARogueLike_ProjectCharacter::AddAmmo(float ConsumableAmmo)
{

	InventoryComponent->AddAmmo(ConsumableAmmo);
	
}

// Function called after damage logic. Activates invulnerability frames
void ARogueLike_ProjectCharacter::TakeDamage(float oldHealth, float currentHealth, float maxHealth, float normalizedHealth)
{

	CurrentInvulnerabilityTime = MaxInvulnerabilityTime;
	HealthComponent->SetCanTakeDamage(false);
	
}

// Deactivate player and notifies GameMode of GameOver
void ARogueLike_ProjectCharacter::KillPlayer(bool isMeleeDamage)
{

	m_IsPlayerAlive = false;
	
	// Hides visible components
	//SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);

	// Stops the Actor from ticking
	SetActorTickEnabled(false);

	InventoryComponent->GetCurrentWeapon()->DisableWeapon(true);

	if (AGameplayGameMode* gameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode()))
	{

		gameMode->GameOver();
		
	}
	
}

// Add interactable to list
void ARogueLike_ProjectCharacter::AddInteractable(UInteractComponent* InteractComponent)
{

	if (!Interactables.Contains(InteractComponent))
	{
		
		Interactables.Add(InteractComponent);
		OnUpdatePlayerCurrentInteractableDelegate.Broadcast(Interactables[m_CurrentInteractable]->InteractableName);
		
	}
	
}

// Remove interactable from list
void ARogueLike_ProjectCharacter::RemoveInteractable(UInteractComponent* InteractComponent)
{

	if (Interactables.Contains(InteractComponent))
	{
		
		Interactables.Remove(InteractComponent);
		
	}

	if (m_CurrentInteractable >= Interactables.Num() || m_CurrentInteractable < 0)
	{
		m_CurrentInteractable = 0;
	}

	Interactables.Num() == 0 ? OnUpdatePlayerCurrentInteractableDelegate.Broadcast("") : OnUpdatePlayerCurrentInteractableDelegate.Broadcast(Interactables[m_CurrentInteractable]->InteractableName);
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARogueLike_ProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARogueLike_ProjectCharacter::Move);
		
		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ARogueLike_ProjectCharacter::Fire);
		
		// Change Weapon
		EnhancedInputComponent->BindAction(ChangeWeaponAction, ETriggerEvent::Triggered, this, &ARogueLike_ProjectCharacter::ChangeWeapon);
		
		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ARogueLike_ProjectCharacter::Reload);

		// Change Interactable
		EnhancedInputComponent->BindAction(ChangeInteracionAction, ETriggerEvent::Triggered, this, &ARogueLike_ProjectCharacter::ChangeInteractable);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ARogueLike_ProjectCharacter::Interact);
		
		// Pause game
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ARogueLike_ProjectCharacter::Pause);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

// Move player based on camera perspective
void ARogueLike_ProjectCharacter::Move(const FInputActionValue& Value)
{
	if (m_IsPlayerAlive && m_CanPlayerMove)
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// find out which way is forward
			const FRotator Rotation = FollowCamera->GetComponentTransform().Rotator();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			//const FVector ForwardDirection(1, 0, 0);
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

// Attack with current weapon
void ARogueLike_ProjectCharacter::Fire(const FInputActionValue& Value)
{
	if (m_IsPlayerAlive)
	{
		InventoryComponent->FireCurrentWeapon();
	}
	
}

// Change current weapon
void ARogueLike_ProjectCharacter::ChangeWeapon(const FInputActionValue& Value)
{

	if (m_IsPlayerAlive && m_CanPlayerMove)
	{		
		InventoryComponent->ChangeWeapon(Value.Get<float>() < 0);
		
		OnUpdatePlayerCurrentWeaponDelegate.Broadcast(InventoryComponent->GetCurrentWeapon());
	}
	
}

// Reload current weapon if possible
void ARogueLike_ProjectCharacter::Reload(const FInputActionValue& Value)
{

	if (m_IsPlayerAlive)
	{
		
		InventoryComponent->GetCurrentWeapon()->Reload();	
			
		OnUpdatePlayerCurrentWeaponDelegate.Broadcast(InventoryComponent->GetCurrentWeapon());
	
	}
	
}

// Change current interatable if possible
void ARogueLike_ProjectCharacter::ChangeInteractable(const FInputActionValue& Value)
{

	if (m_IsPlayerAlive && Interactables.Num() > 1)
	{		
		Value.Get<float>() < 0 ? m_CurrentInteractable-- : m_CurrentInteractable++;
		
		if (m_CurrentInteractable < 0)
		{
			m_CurrentInteractable = Interactables.Num() - 1;
		}
		else if (m_CurrentInteractable >= Interactables.Num())
		{
			m_CurrentInteractable = 0;
		}
		
		OnUpdatePlayerCurrentInteractableDelegate.Broadcast(Interactables[m_CurrentInteractable]->InteractableName);
	}

	
}

// Interact with current item
void ARogueLike_ProjectCharacter::Interact(const FInputActionValue& Value)
{

	if (m_IsPlayerAlive && Interactables.Num() >= 1)
	{
		
		Interactables[m_CurrentInteractable]->Interact(this);
	
	}
	
}

// Pause / Unpause game
void ARogueLike_ProjectCharacter::Pause(const FInputActionValue& Value)
{

	OnPlayerPause.Broadcast();
	
}

// Activate / Deactivate melee collider
void ARogueLike_ProjectCharacter::UseMeleeCollider(float ExtraRange, bool isMeleeColliderActive)
{

	if (isMeleeColliderActive)
	{
		
		FVector NewScale = FVector(ExtraRange, ExtraRange, MeleeAttackMeshCollider->GetRelativeScale3D().Z);
		MeleeAttackMeshCollider->SetRelativeScale3D(NewScale);
		
	}
	
	m_CanPlayerMove = !isMeleeColliderActive;
	MeleeAttackMeshCollider->SetVisibility(isMeleeColliderActive, false);
	MeleeAttackMeshCollider->SetGenerateOverlapEvents(isMeleeColliderActive);
	
}
