// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
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

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Fire point */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USceneComponent* FirePoint;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Change Weapon Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeWeaponAction;

	/** Force Damage Input Action (for debug purposes) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ForceDamageAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_IsPlayerAlive = true;

public:
	
	ARogueLike_ProjectCharacter();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float oldHealth, float currentHealth, float normalizedHealth);
	
	UFUNCTION(BlueprintCallable)
	void KillPlayer();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdatePlayerCurrentWeapon, ABaseWeapon*, newWeapon);
	UPROPERTY(BlueprintAssignable, Category = "Configuration")
	FUpdatePlayerCurrentWeapon OnUpdatePlayerCurrentWeaponDelegate;
	
protected:
	
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
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

	/** Called for force damage input (debug purpose) **/
	void ForceDamage(const FInputActionValue& Value);
	
public:
	
	/** Inventory component **/
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Components", meta = ( AllowPrivateAccess = "true" ))
	UInventoryComponent* InventoryComponent;
	
	/** Health component **/
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Components", meta = ( AllowPrivateAccess = "true" ))
	UHealthComponent* HealthComponent;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};

