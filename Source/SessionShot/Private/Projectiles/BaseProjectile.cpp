// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Projectiles/BaseProjectile.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile::ABaseProjectile()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxCollision);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("UParticleSystemComponent");
	ParticleSystemComponent->SetupAttachment(GetRootComponent());

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");

	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	PrimaryActorTick.bCanEverTick = false;

	MovementComponent->MaxSpeed = 5000.0f;
	MovementComponent->InitialSpeed = 5000.0f;
	MovementComponent->bRotationFollowsVelocity = true;

	bReplicates = true;
	SetReplicateMovement(true);
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileHit);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	BoxCollision->IgnoreActorWhenMoving(GetOwner(), true);
	SetLifeSpan(ProjectileLifeSpan);

}

void ABaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetOwner())
	{
		OtherActor->TakeDamage(Damage, FDamageEvent{}, GetOwnerController(), this);
	}

	Destroy();
}

AController* ABaseProjectile::GetOwnerController() const
{
	if (!GetOwner()) return nullptr;

	const auto Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return nullptr;

	return Pawn->GetController();
}
