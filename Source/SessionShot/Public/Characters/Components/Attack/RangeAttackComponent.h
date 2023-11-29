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
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack FX")
    USoundCue* SoundShot;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack FX")
    UParticleSystem* CascadeParticle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack FX")
    FName ParticleAttachName = "MuzzleSocket";
};

USTRUCT(BlueprintType)
struct FProjectileData
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
    TSubclassOf<ABaseProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
    FFXData FXData;
};

UCLASS()
class SESSIONSHOT_API URangeAttackComponent : public UBaseAttackComponent
{
    GENERATED_BODY()

public:
    virtual void Attack() override;

    UFUNCTION(BlueprintCallable, Category = "Range Attack")
    void Shoot(const FProjectileData& ProjectileData);

protected:
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

    bool SpawnProjectile(const FProjectileData& ProjectileData);

    void MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);

    bool GetPlayerViewPoint(FVector& TraceStart, FRotator& TraceRotation);

    FVector GetSocketWorldLocation(const FName& SocketName) const;

    UFUNCTION(Category = "FX")
    void SpawnShotFX();

    UFUNCTION()
    void OnRep_BurstCounter();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
    float ShootRate = 0.35f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
    float TraceDistance = 20000.0f;

    UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter)
    int32 BurstCounter;  // Currently needed for FX replication

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
    FProjectileData DefaultProjectileData;

    UPROPERTY(Replicated)
    FFXData CurrentFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Range Attack")
    FName MuzzleSocketName = "MuzzleSocket";
};
