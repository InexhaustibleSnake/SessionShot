// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attack/BaseAttackComponent.h"
#include "Characters/BaseCharacter.h"

UBaseAttackComponent::UBaseAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    SetComponentTickEnabled(false);

    SetIsReplicatedByDefault(true);
}

void UBaseAttackComponent::Attack()
{
    if (!GetOwner()->HasAuthority())
    {
        Server_Attack();
    }
}

void UBaseAttackComponent::Server_Attack_Implementation()
{
    Attack();
}

AController* UBaseAttackComponent::GetOwnerController() const
{
    const auto OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return nullptr;

    return OwnerPawn->GetController();
}
