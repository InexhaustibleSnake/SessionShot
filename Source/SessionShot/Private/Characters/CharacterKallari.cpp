// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes


#include "Characters/CharacterKallari.h"

void ACharacterKallari::ServerAttack_Implementation()
{
	auto MaxAttackIndex = ComboAttackMap.Num();

	CurrentAttackIndex = ++CurrentAttackIndex % MaxAttackIndex;
	
	UAnimMontage** AttackMontage = ComboAttackMap.Find(CurrentAttackIndex);

	if (!AttackMontage) return;

	PlayAnimMontage(*AttackMontage);
}
