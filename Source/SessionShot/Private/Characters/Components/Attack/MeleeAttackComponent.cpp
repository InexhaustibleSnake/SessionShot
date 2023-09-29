// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attack/MeleeAttackComponent.h"
#include "Characters/BaseCharacter.h"

void UMeleeAttackComponent::Server_Attack_Implementation()
{
	if (ComboAttackMap.IsEmpty()) return;

	int32 MaxAttackIndex = ComboAttackMap.Num();

	CurrentAttackIndex = ++CurrentAttackIndex % MaxAttackIndex;

	UAnimMontage** AttackMontage = ComboAttackMap.Find(CurrentAttackIndex);
	if (!AttackMontage) return;

	if (!GetOwner()) return;

	auto OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (!OwnerCharacter) return;

	OwnerCharacter->Multicast_PlayAnimMontage(*AttackMontage);
}
