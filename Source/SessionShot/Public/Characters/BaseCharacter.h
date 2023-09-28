// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

UCLASS()
class SESSIONSHOT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	void AddMovement(const FInputActionValue& Value);

	virtual void Attack();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, Category = "Attack")
	virtual void ServerAttack();
	virtual void ServerAttack_Implementation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	int32 CurrentAttackIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	TMap<int32, UAnimMontage*> ComboAttackMap;

};
