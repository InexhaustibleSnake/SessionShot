// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MainPlayerController.generated.h"

class UInputMappingContext;
class UPlayerInputData;

UCLASS()
class SESSIONSHOT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void SetupInputComponent();
	void AddControlInput(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control")
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control")
    UPlayerInputData* InputActions;

};
