// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Projectiles/BaseProjectile.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
