// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
