// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attack/RangeAttackComponent.h"
#include "Projectiles/BaseProjectile.h"
#include "Components/SkeletalMeshComponent.h"

void URangeAttackComponent::Server_Attack_Implementation()
{
	SpawnProjectile();
}

void URangeAttackComponent::SpawnProjectile()
{
	if (!GetWorld()) return;

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;

	MakeTrace(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());

	auto SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileType, SpawnTransform);
	if (SpawnedProjectile)
	{
		SpawnedProjectile->SetShotDirection(Direction);
		SpawnedProjectile->SetOwner(GetOwner());
		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
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

FVector URangeAttackComponent::GetMuzzleWorldLocation() const
{
	if (!GetOwner()) return FVector::ZeroVector;

	const auto CharacterMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();

	return CharacterMesh ? CharacterMesh->GetSocketLocation(MuzzleSocketName) : FVector::ZeroVector;
}
