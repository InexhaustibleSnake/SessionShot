// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Characters/Components/Attack/MeleeAttackComponent.h"
#include "Characters/Components/Attack/RangeAttackComponent.h"
#include "Characters/Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetMesh());

	MainCamera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
	MainCamera->SetupAttachment(SpringArmComponent);

	MeleeAttackComponent = CreateDefaultSubobject<UMeleeAttackComponent>("MeleeAttackComponent");
	RangeAttackComponent = CreateDefaultSubobject<URangeAttackComponent>("RangeAttackComponent");
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 350.0f;

	FVector SpringArmComponentLocation;
	SpringArmComponentLocation.Set(0.0f, 0.0f, 180.0f);
	SpringArmComponent->SetWorldLocation(SpringArmComponentLocation);

	FVector SpringArmSocketOffset;
	SpringArmSocketOffset.Set(0.0f, 50.0f, 0.0f);
	SpringArmComponent->SocketOffset = SpringArmSocketOffset;

	SpringArmComponent->bEnableCameraLag = true;
	float CameraLagSpeed = 12.5f;
	SpringArmComponent->CameraLagSpeed = CameraLagSpeed;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = true;

	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 12.5f;

	SetReplicates(true);

	GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CheckActorComponents();

	HealthComponent->OnDeath.AddDynamic(this, &ABaseCharacter::OnDeath);

	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
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
	if (Aiming)
	{
		RangeAttackComponent->Attack();
	}
	else
	{
		MeleeAttackComponent->Attack();
	}
}

void ABaseCharacter::OnPlayerAiming()
{
	if (!HasAuthority())
	{
		ServerOnPlayerAiming();
	}

	Aiming = !Aiming;
	OnRep_Aiming();

	if (IsLocallyControlled())
	{
		const auto PlayerController = Cast<APlayerController>(GetController());
		if (!PlayerController) return;
		PlayerController->PlayerCameraManager->SetFOV(Aiming ? AimFOV : NonAimFOV);
	}
}

void ABaseCharacter::ServerOnPlayerAiming_Implementation()
{
	OnPlayerAiming();
}

void ABaseCharacter::OnRep_Aiming()
{
	bUseControllerRotationYaw = Aiming;
	GetCharacterMovement()->MaxWalkSpeed = Aiming ? WalkingSpeed : DefaultSpeed;
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

float ABaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABaseCharacter, Aiming, COND_SkipOwner);
}
