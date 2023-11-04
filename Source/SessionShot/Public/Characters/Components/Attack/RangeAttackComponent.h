// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/Attack/BaseAttackComponent.h"
#include "RangeAttackComponent.generated.h"

class ABaseProjectile;
class USoundCue;
class UParticleSystem;

USTRUCT(BlueprintType)
struct FFXData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	USoundCue* SoundShot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	UParticleSystem* CascadeParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	FName ParticleAttachName;

};

UCLASS()
class SESSIONSHOT_API URangeAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:
	virtual void Attack() override;

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void SpawnProjectile();

	void MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);

	bool GetPlayerViewPoint(FVector& TraceStart, FRotator& TraceRotation);

	FVector GetMuzzleWorldLocation() const;

	UFUNCTION( Category = "FX")
	void SpawnShotFX();

	UFUNCTION()
	void OnRep_BurstCounter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	float ShootRate = 0.35f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	float TraceDistance = 20000.0f;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter)
	int32 BurstCounter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	TSubclassOf<ABaseProjectile> ProjectileType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	FFXData ProjectileFXData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
	FName MuzzleSocketName = "MuzzleSocket";
};
