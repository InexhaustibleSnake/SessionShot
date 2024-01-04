// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/Components/Attack/MeleeAttackComponent.h"
#include "Characters/Components/Attack/RangeAttackComponent.h"
#include "Characters/Components/HealthComponent.h"
#include "Components/TimelineComponent.h"

#include "Characters/Components/AbilityComponent.h"
#include "Characters/Abilities/BaseAbility.h"
#include "Characters/Components/Attributes/BaseAttributeSet.h"
#include "Curves/CurveFloat.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Player/Input/CharacterInputData.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"

#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    FName SpringArmAttachmentName = "head";
    SpringArmComponent->SetupAttachment(GetMesh(), SpringArmAttachmentName);

    MainCamera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
    MainCamera->SetupAttachment(SpringArmComponent);

    MeleeAttackComponent = CreateDefaultSubobject<UMeleeAttackComponent>("MeleeAttackComponent");
    RangeAttackComponent = CreateDefaultSubobject<URangeAttackComponent>("RangeAttackComponent");

    AbilityComponent = CreateDefaultSubobject<UAbilityComponent>("AbilityComponent");
    AbilityComponent->SetIsReplicated(true);
    AbilityComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = NonAimSpringArmLenght;

    FVector SpringArmSocketOffset;
    SpringArmSocketOffset.Set(0.0f, 50.0f, 0.0f);
    SpringArmComponent->SocketOffset = SpringArmSocketOffset;

    SpringArmComponent->bEnableCameraLag = true;
    float CameraLagSpeed = 12.5f;
    SpringArmComponent->CameraLagSpeed = CameraLagSpeed;

    GetCharacterMovement()->MaxAcceleration = 1024.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = true;

    SpringArmComponent->bEnableCameraLag = true;
    SpringArmComponent->CameraLagSpeed = 12.5f;

    SetReplicates(true);

    GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    NetUpdateFrequency = 100.0f;  // Character needs to be updated at a high frequency.

    SetupInput();
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    CheckActorComponents();

    GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

    InitializeAttributes();
    HealthComponent->InitializeWithAbilityComponent(AbilityComponent);

    if (AimingCurve)
    {
        FOnTimelineFloat SpringArmUpdate;
        SpringArmUpdate.BindUFunction(this, FName("ChangeSpringArmLength"));

        AimingTimeline.AddInterpFloat(AimingCurve, SpringArmUpdate);
        AimingTimeline.SetLooping(false);
        AimingTimeline.SetTimelineLength(1.0f);
        AimingTimeline.SetPlayRate(AimingTime);
    }
}

void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AimingTimeline.TickTimeline(DeltaTime);
}

void ABaseCharacter::ChangeSpringArmLength(float Alpha)
{
    SpringArmComponent->TargetArmLength = FMath::Lerp(NonAimSpringArmLenght, AimSpringArmLenght, Alpha);
}

void ABaseCharacter::SetupInput()
{
    if (!InputActions)
    {
        static ConstructorHelpers::FObjectFinder<UCharacterInputData> DA_PlayerInput(
            TEXT("/Script/SessionShot.CharacterInputData'/Game/SessionShot/Player/Input/DA_CharacterInput.DA_CharacterInput'"));

        if (DA_PlayerInput.Succeeded()) InputActions = DA_PlayerInput.Object;
    }

    if (!InputMapping)
    {
        static ConstructorHelpers::FObjectFinder<UInputMappingContext> IA_InputMapping(
            TEXT("/Script/EnhancedInput.InputMappingContext'/Game/SessionShot/Player/Input/IMC_Main.IMC_Main'"));

        if (IA_InputMapping.Succeeded()) InputMapping = IA_InputMapping.Object;
    }
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (!InputComponent || !InputActions || !InputMapping) return;

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController) return;

    UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

    if (!EnhancedInputSubsystem) return;

    EnhancedInputSubsystem->ClearAllMappings();
    EnhancedInputSubsystem->AddMappingContext(InputMapping, 0);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(InputActions->Movement, ETriggerEvent::Triggered, this, &ABaseCharacter::AddMovement);

    EnhancedInputComponent->BindAction(InputActions->Jump, ETriggerEvent::Triggered, this, &ABaseCharacter::Jump);

    EnhancedInputComponent->BindAction(InputActions->Attack, ETriggerEvent::Started, this, &ABaseCharacter::Attack);

    EnhancedInputComponent->BindAction(InputActions->Aim, ETriggerEvent::Started, this, &ABaseCharacter::OnPlayerAiming);
    EnhancedInputComponent->BindAction(InputActions->Aim, ETriggerEvent::Completed, this, &ABaseCharacter::OnPlayerAiming);

    if (!AbilityComponent) return;

    FTopLevelAssetPath EnumPath = FTopLevelAssetPath(FName("/Script/SessionShot"), FName("EAbilityInputID"));

    if (EnumPath.IsNull()) return;

    const FGameplayAbilityInputBinds Binds(
        "Confirm", "Cancel", EnumPath, static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));
    AbilityComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
}

void ABaseCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (!AbilityComponent) return;
    AbilityComponent->InitAbilityActorInfo(this, this);
    InitializeAttributes();
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    AbilityComponent->InitAbilityActorInfo(this, this);
    GiveAbilities();  // Only server needs to give abilities
}

void ABaseCharacter::InitializeAttributes()
{
    if (!AbilityComponent && !InitialEffects.IsEmpty()) return;

    FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    for (auto InitialEffect : InitialEffects)
    {
        FGameplayEffectSpecHandle SpecHandle = AbilityComponent->MakeOutgoingSpec(InitialEffect, GetCharacterLevel(), EffectContext);

        if (!SpecHandle.IsValid()) continue;

        AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
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
    Aiming ? RangeAttackComponent->Attack() : MeleeAttackComponent->Attack();
}

void ABaseCharacter::OnPlayerAiming(const FInputActionValue& Value)
{
    Aiming = !Aiming;
    OnRep_Aiming();

    if (IsLocallyControlled())
    {
        Aiming ? AimingTimeline.Play() : AimingTimeline.Reverse();
    }

    if (!HasAuthority()) ServerOnPlayerAiming(Value);
}

void ABaseCharacter::ServerOnPlayerAiming_Implementation(const FInputActionValue& Value)
{
    OnPlayerAiming(Value);
}

void ABaseCharacter::OnRep_Aiming()
{
    bUseControllerRotationYaw = Aiming;
    GetCharacterMovement()->MaxWalkSpeed = Aiming ? WalkingSpeed : DefaultSpeed;
}

void ABaseCharacter::GiveAbilities()
{
    if (!HasAuthority() || !AbilityComponent) return;

    for (TSubclassOf<UBaseAbility>& OneAbility : DefaultAbilities)
    {
        if (!OneAbility) continue;

        AbilityComponent->GiveAbility(
            FGameplayAbilitySpec(OneAbility, GetAbilityLevel(), static_cast<int32>(OneAbility.GetDefaultObject()->AbilityInputID), this));
    }
}

void ABaseCharacter::Multicast_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
    PlayAnimMontage(AnimMontage);
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

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
    return AbilityComponent;
}

UBaseAttributeSet* ABaseCharacter::GetAttributeSetBase() const
{
    return AttributeSet;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(ABaseCharacter, Aiming, COND_SkipOwner);
}
