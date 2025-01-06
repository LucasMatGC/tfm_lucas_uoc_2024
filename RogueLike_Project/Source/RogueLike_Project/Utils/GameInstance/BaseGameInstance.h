// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RogueLike_Project/RogueLike_ProjectCharacter.h"
#include "BaseGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FWeaponVariables
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int WeaponsAmmo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FUpgradeStruct> ListOfUpgrades;
	
};

USTRUCT(BlueprintType)
struct FGameVariables
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float PlayerHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CurrentGameTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FUpgradeStruct> ListOfCommonUpgrades;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<int, FWeaponVariables> WeaponInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsGameFinished = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsGameWon = false;
	
};


/**
 * 
 */
UCLASS()
class ROGUELIKE_PROJECT_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
	int PredefinedSeed = -1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AudioMaster = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AudioMusic = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AudioSFX = 0.2f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Configuration")
	FRandomStream RandomStream;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Configuration")
	FGameVariables SavedGameVariables;

public:
	
	// Sets default values for this actor's properties
	UBaseGameInstance();
	
	UFUNCTION(BlueprintCallable)
	void SetGameVariables(APlayerController* CurrentPlayer, float CurrentGameTime, bool IsGameFinished, bool IsGameWon);
	
	UFUNCTION(BlueprintCallable)
	FGameVariables GetGameVariables();
	
	UFUNCTION(BlueprintCallable)
	int RandomRangeInt(int Min, int Max);
	
	UFUNCTION(BlueprintCallable)
	float RandomRangeFloat(float Min, float Max);

	UFUNCTION(BlueprintCallable)
	void SetSeed();
	
};
