// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Characters/Components/Attack/MeleeAttackComponent.h"
#include "Characters/Components/Attack/RangeAttackComponent.h"
#include "Characters/Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	MeleeAttackComponent = CreateDefaultSubobject<UMeleeAttackComponent>("MeleeAttackComponent");
	RangeAttackComponent = CreateDefaultSubobject<URangeAttackComponent>("RangeAttackComponent");
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

	SetReplicates(true);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnDeath.AddDynamic(this, &ABaseCharacter::OnDeath);
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
	MeleeCharacter ? MeleeAttackComponent->Attack() : RangeAttackComponent->Attack();
}

void ABaseCharacter::SecondaryAttack()
{
	MeleeCharacter ? RangeAttackComponent->Attack() : MeleeAttackComponent->Attack();
}

void ABaseCharacter::Multicast_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}

void ABaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}