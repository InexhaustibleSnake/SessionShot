// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Characters/Components/Attack/MeleeAttackComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	MeleeAttackComponent = CreateDefaultSubobject<UMeleeAttackComponent>("MeleeAttackComponent");

	SetReplicates(true);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseCharacter::AddMovement(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ABaseCharacter::Attack()
{
	MeleeAttackComponent->Attack();
}

void ABaseCharacter::Multicast_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}
