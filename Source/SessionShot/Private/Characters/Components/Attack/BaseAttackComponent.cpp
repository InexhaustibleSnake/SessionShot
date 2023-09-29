// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attack/BaseAttackComponent.h"
#include "Characters/BaseCharacter.h"

UBaseAttackComponent::UBaseAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UBaseAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBaseAttackComponent::Attack()
{
	Server_Attack();
}

void UBaseAttackComponent::Server_Attack_Implementation()
{
}
