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
#include "Kismet/GameplayStatics.h"
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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ARogueLike_ProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

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

	if (AGameplayGameMode* gameMode = Cast<AGameplayGameMode>(GetWorld()->GetAuthGameMode()))
	{

		gameMode->PreparePlayer();
		
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

void ARogueLike_ProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentInvulnerabilityTime > 0.0f)
	{

		CurrentInvulnerabilityTime -= DeltaTime;

		if (CurrentInvulnerabilityTime <= 0.0f && m_IsPlayerAlive)
		{

			HealthComponent->SetCanTakeDamage(true);
			
		}
		
	}
	
	if (m_IsPlayerAlive && m_CanPlayerMove)
	{

		
		//Add Input Mapping Context
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{

			FVector2D mousePos = FVector2D::ZeroVector;
			FHitResult* hit = new FHitResult();
			FVector startTrace;
			FVector direction;
			FCollisionQueryParams* params = new FCollisionQueryParams();
			
			PlayerController->GetMousePosition(mousePos.X, mousePos.Y);

			//TODO: Desactivar control de mando. En caso de usarse, Detectar cuando se usa mando para desactivar esta porcion
			// Y usar joystick dcho para rotar. Usar la funcion "look" y esto solo para alimentar el valor en caso de que sea raton
			PlayerController->DeprojectScreenPositionToWorld(mousePos.X, mousePos.Y, startTrace, direction);

			FVector endTrace = direction * (CameraBoom->TargetArmLength * 30) + startTrace;

			if (GetWorld()->LineTraceSingleByChannel(*hit, startTrace, endTrace, ECollisionChannel::ECC_Visibility, *params))
			{

				if (hit->GetActor() != nullptr)
				{

					SetActorRotation( FRotator(
						0,
						UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), hit->Location).Yaw,
						0));
					
				}
				
			}
			
		}
		
	}
	
}

void ARogueLike_ProjectCharacter::ApplyMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Apply Damage!!!!");
	InventoryComponent->ApplyMeleeDamage(OtherActor);
	
}

void ARogueLike_ProjectCharacter::HealPlayer(float ConsumableHealth)
{

	HealthComponent->Heal(ConsumableHealth);
	
}

void ARogueLike_ProjectCharacter::AddAmmo(float ConsumableAmmo)
{

	InventoryComponent->AddAmmo(ConsumableAmmo);
	
}

void ARogueLike_ProjectCharacter::TakeDamage(float oldHealth, float currentHealth, float normalizedHealth)
{

	CurrentInvulnerabilityTime = MaxInvulnerabilityTime;
	HealthComponent->SetCanTakeDamage(false);
	
}

void ARogueLike_ProjectCharacter::KillPlayer()
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

void ARogueLike_ProjectCharacter::AddInteractable(UInteractComponent* InteractComponent)
{

	if (!Interactables.Contains(InteractComponent))
	{
		
		Interactables.Add(InteractComponent);
		OnUpdatePlayerCurrentInteractableDelegate.Broadcast(Interactables[m_CurrentInteractable]->InteractableName);
		
	}
	
}

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

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARogueLike_ProjectCharacter::Look);
		
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
		
		// Debug Take Damage
		EnhancedInputComponent->BindAction(ForceDamageAction, ETriggerEvent::Triggered, this, &ARogueLike_ProjectCharacter::ForceDamage);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

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

void ARogueLike_ProjectCharacter::Look(const FInputActionValue& Value)
{

	if (m_IsPlayerAlive && m_CanPlayerMove)
	{
		// input is a Vector2D
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{

			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
			
		}
	}
}

void ARogueLike_ProjectCharacter::Fire(const FInputActionValue& Value)
{
	if (m_IsPlayerAlive)
	{
		InventoryComponent->FireCurrentWeapon();
	}
	
}

void ARogueLike_ProjectCharacter::ChangeWeapon(const FInputActionValue& Value)
{

	if (m_IsPlayerAlive && m_CanPlayerMove)
	{		
		InventoryComponent->ChangeWeapon(Value.Get<float>() < 0);
		
		OnUpdatePlayerCurrentWeaponDelegate.Broadcast(InventoryComponent->GetCurrentWeapon());
	}
	
}

void ARogueLike_ProjectCharacter::Reload(const FInputActionValue& Value)
{

	if (m_IsPlayerAlive)
	{
		
		InventoryComponent->GetCurrentWeapon()->Reload();	
			
		OnUpdatePlayerCurrentWeaponDelegate.Broadcast(InventoryComponent->GetCurrentWeapon());
	
	}
	
}

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

void ARogueLike_ProjectCharacter::Interact(const FInputActionValue& Value)
{

	if (m_IsPlayerAlive && Interactables.Num() >= 1)
	{
		
		Interactables[m_CurrentInteractable]->Interact(this);
	
	}
	
}

void ARogueLike_ProjectCharacter::Pause(const FInputActionValue& Value)
{

	OnPlayerPause.Broadcast();
	
}

void ARogueLike_ProjectCharacter::ForceDamage(const FInputActionValue& Value)
{

	if (m_IsPlayerAlive)
	{
		
		float forceDamageValue = 20.0f;
	
		UGameplayStatics::ApplyDamage(this, forceDamageValue, GetController(), this, UDamageType::StaticClass());
	
	}
	
}

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
