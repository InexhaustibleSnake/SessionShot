// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UParticleSystemComponent;
class UGameplayEffect;

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
    TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Projectile")
    TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Projectile")
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Projectile")
	float ProjectileLifeSpan = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Projectile")
    TSubclassOf<UGameplayEffect> ProjectileEffect;

private:
	FVector ShotDirection;

};
