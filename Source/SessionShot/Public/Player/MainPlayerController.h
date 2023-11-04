// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MainPlayerController.generated.h"

class ABaseCharacter;
class UInputMappingContext;
class UPlayerInputData;

UCLASS()
class SESSIONSHOT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;

	ABaseCharacter* GetPossessedCharacter() const;

	void AddControlInput(const FInputActionValue& Value);
	void AddMovementInput(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);

	void Aim(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control")
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control")
    UPlayerInputData* InputActions;

};
