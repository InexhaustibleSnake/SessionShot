// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UParticleSystemComponent;

UCLASS()
class SESSIONSHOT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	AController* GetOwnerController() const;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Projectile")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Projectile")
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Projectile")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Projectile")
	float Damage = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Projectile")
	float ProjectileLifeSpan = 10.0f;

private:
	FVector ShotDirection;

};
