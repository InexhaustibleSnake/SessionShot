// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/Attack/BaseAttackComponent.h"
#include "RangeAttackComponent.generated.h"

class ABaseProjectile;

UCLASS()
class SESSIONSHOT_API URangeAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
protected:
	virtual void Server_Attack_Implementation() override;

	void SpawnProjectile();

	void MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);

	bool GetPlayerViewPoint(FVector& TraceStart, FRotator& TraceRotation);

	FVector GetMuzzleWorldLocation() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	float ShootRate = 0.35f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	float TraceDistance = 20000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	TSubclassOf<ABaseProjectile> ProjectileType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	FName MuzzleSocketName = "MuzzleSocket";
};
