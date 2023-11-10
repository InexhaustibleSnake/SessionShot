// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SessionShot/SessionShot.h"
#include "BaseAbility.generated.h"

UCLASS()
class SESSIONSHOT_API UBaseAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UBaseAbility();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	EAbilityInputID AbilityInputID = EAbilityInputID::None;
	
};
