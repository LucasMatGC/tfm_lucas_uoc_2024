#pragma once

UENUM(BlueprintType)
enum class UEnemyState : uint8
{
	Patrolling = 0	UMETA(DisplayName = "Patrolling"),
	Attacking = 1	UMETA(DisplayName = "Attacking")
};
