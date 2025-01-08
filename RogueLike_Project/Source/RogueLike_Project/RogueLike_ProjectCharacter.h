// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "Components/InteractComponent.h"
#include "Components/Player/InventoryComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "RogueLike_ProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ARogueLike_ProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Fire point */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USceneComponent* FirePoint;

	/** Melee mesh collider */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeleeAttackMeshCollider;
	
	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	/** Move Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Change Weapon Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeWeaponAction;

	/** Reload Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	/** Change Interacion Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeInteracionAction;

	/** Reload Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Pause/Unpause Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	/** Force Damage Input Action (for debug purposes) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ForceDamageAction;
	
	/** Inventory component **/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* InventoryComponent;
	
	/** Health component **/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	/** Interactable components list **/
	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TArray<UInteractComponent*> Interactables;

	/** Interactable components list **/
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
	float MaxInvulnerabilityTime = 2.0f;

	/** Interactable components list **/
	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	float CurrentInvulnerabilityTime = 0.0f;

public:
	
	ARogueLike_ProjectCharacter();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float oldHealth, float currentHealth, float maxHealth, float normalizedHealth);
	
	UFUNCTION(BlueprintCallable)
	void KillPlayer(bool isMeleeDamage);

	UFUNCTION(BlueprintCallable)
	void AddInteractable(UInteractComponent* InteractComponent);

	UFUNCTION(BlueprintCallable)
	void RemoveInteractable(UInteractComponent* InteractComponent);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerReady);
	UPROPERTY(BlueprintAssignable, Category = "Configuration")
	FPlayerReady OnPlayerReady;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerPause);
	UPROPERTY(BlueprintAssignable, Category = "Configuration")
	FPlayerPause OnPlayerPause;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdatePlayerCurrentWeapon, ABaseWeapon*, newWeapon);
	UPROPERTY(BlueprintAssignable, Category = "Configuration")
	FUpdatePlayerCurrentWeapon OnUpdatePlayerCurrentWeaponDelegate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdatePlayerCurrentInteractable, FName, interactName);
	UPROPERTY(BlueprintAssignable, Category = "Configuration")
	FUpdatePlayerCurrentInteractable OnUpdatePlayerCurrentInteractableDelegate;
	
public:
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	void HealPlayer(float ConsumableHealth);

	UFUNCTION(BlueprintCallable)
	void AddAmmo(float ConsumableAmmo);

	UFUNCTION(BlueprintCallable)
	void UseMeleeCollider(float ExtraRange, bool isMeleeColliderActive);
	
protected:
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void ApplyMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for fire input **/
	void Fire(const FInputActionValue& Value);

	/** Called for change weapon input **/
	void ChangeWeapon(const FInputActionValue& Value);

	/** Called for weapon reload input **/
	void Reload(const FInputActionValue& Value);

	/** Called for change interactable input **/
	void ChangeInteractable(const FInputActionValue& Value);

	/** Called for interaction input **/
	void Interact(const FInputActionValue& Value);

	/** Called for Pause/Unpause input **/
	void Pause(const FInputActionValue& Value);

	/** Called for force damage input (debug purpose) **/
	void ForceDamage(const FInputActionValue& Value);

private:

	int m_CurrentInteractable = 0;
	
	UPROPERTY(VisibleDefaultsOnly)
	bool m_IsPlayerAlive = true;
	
	UPROPERTY(VisibleDefaultsOnly)
	bool m_CanPlayerMove = true;
	
};

