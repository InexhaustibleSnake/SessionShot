// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Player/MainPlayerController.h"
#include "Characters/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Player/Input/PlayerInputData.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent || !GetLocalPlayer()) return;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!EnhancedInputSubsystem) return;

	EnhancedInputSubsystem->ClearAllMappings();
	EnhancedInputSubsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(InputActions->Turn, ETriggerEvent::Triggered, this, &AMainPlayerController::AddControlInput);
	EnhancedInputComponent->BindAction(InputActions->Movement, ETriggerEvent::Triggered, this, &AMainPlayerController::AddMovementInput);

	EnhancedInputComponent->BindAction(InputActions->Jump, ETriggerEvent::Triggered, this, &AMainPlayerController::Jump);
}

void AMainPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

ABaseCharacter* AMainPlayerController::GetPossessedCharacter() const
{
	if (!GetPawn()) return nullptr;

	return Cast<ABaseCharacter>(GetPawn());
}

void AMainPlayerController::AddControlInput(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "PC");
	FVector2D ControlVector = Value.Get<FVector2D>();

	AddPitchInput(ControlVector.Y);
	AddYawInput(ControlVector.X);
}

void AMainPlayerController::AddMovementInput(const FInputActionValue& Value)
{
	if (!GetPossessedCharacter()) return;

	FVector2D ControlVector = Value.Get<FVector2D>();

	GetPossessedCharacter()->AddMovement(Value);
}

void AMainPlayerController::Jump(const FInputActionValue& Value)
{
	if (!GetPossessedCharacter()) return;

	GetPossessedCharacter()->Jump();
}
