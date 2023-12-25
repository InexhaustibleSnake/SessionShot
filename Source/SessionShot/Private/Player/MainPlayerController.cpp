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

    UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if (!EnhancedInputSubsystem) return;

    EnhancedInputSubsystem->ClearAllMappings();
    EnhancedInputSubsystem->AddMappingContext(InputMapping, 0);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(InputActions->Turn, ETriggerEvent::Triggered, this, &AMainPlayerController::AddControlInput);
}

void AMainPlayerController::AddControlInput(const FInputActionValue& Value)
{
	FVector2D ControlVector = Value.Get<FVector2D>();

	AddPitchInput(ControlVector.Y);
	AddYawInput(ControlVector.X);
}
