// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attack/RangeAttackComponent.h"
#include "Projectiles/BaseProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void URangeAttackComponent::Attack()
{
    Super::Attack();

    if (!CanAttack) return;

    Shoot(DefaultProjectileData);
}

void URangeAttackComponent::Shoot(const FProjectileData& ProjectileData)
{
    if (!GetOwner()->HasAuthority()) return;

    if (!SpawnProjectile(ProjectileData)) return;

    CurrentFXData = ProjectileData.FXData;

    ++BurstCounter;
    OnRep_BurstCounter();
}

bool URangeAttackComponent::SpawnProjectile(const FProjectileData& ProjectileData)
{
    if (!GetWorld() || !ProjectileData.ProjectileClass) return false;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return false;

    FHitResult HitResult;

    MakeTrace(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetSocketWorldLocation(MuzzleSocketName)).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetSocketWorldLocation(MuzzleSocketName));

    auto SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileData.ProjectileClass, SpawnTransform);
    if (!SpawnedProjectile) return false;

    SpawnedProjectile->SetShotDirection(Direction);
    SpawnedProjectile->SetOwner(GetOwner());
    SpawnedProjectile->FinishSpawning(SpawnTransform);

    return true;
}

bool URangeAttackComponent::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    FVector ShootDirection = ViewRotation.Vector();

    TraceStart = ViewLocation;
    TraceEnd = TraceStart + ShootDirection * TraceDistance;

    return true;
}

bool URangeAttackComponent::GetPlayerViewPoint(FVector& TraceStart, FRotator& TraceRotation)
{
    if (!GetOwner()) return false;

    const auto Pawn = Cast<APawn>(GetOwner());
    if (!Pawn) return false;

    const auto PawnController = Pawn->GetController();
    if (!PawnController) return false;

    PawnController->GetPlayerViewPoint(TraceStart, TraceRotation);

    return true;
}

void URangeAttackComponent::MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;

    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
}

FVector URangeAttackComponent::GetSocketWorldLocation(const FName& SocketName) const
{
    if (!GetOwner()) return FVector::ZeroVector;

    const auto CharacterMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();

    return CharacterMesh ? CharacterMesh->GetSocketLocation(SocketName) : FVector::ZeroVector;
}

void URangeAttackComponent::SpawnShotFX()
{
    auto OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter || !OwnerCharacter->GetMesh()) return;

    UGameplayStatics::SpawnEmitterAttached(CurrentFXData.CascadeParticle, OwnerCharacter->GetMesh(), CurrentFXData.ParticleAttachName);
}

void URangeAttackComponent::OnRep_BurstCounter()
{
    if (GetNetMode() != ENetMode::NM_DedicatedServer)
    {
        SpawnShotFX();
    }
}

void URangeAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URangeAttackComponent, BurstCounter);
    DOREPLIFETIME(URangeAttackComponent, CurrentFXData);
}