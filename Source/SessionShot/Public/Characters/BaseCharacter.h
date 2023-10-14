// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

class UMeleeAttackComponent;
class URangeAttackComponent;
class UHealthComponent;

UCLASS()
class SESSIONSHOT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	void AddMovement(const FInputActionValue& Value);

	virtual void Attack();

	virtual void SecondaryAttack();

	UFUNCTION(NetMulticast, UnReliable, Category = "Animations")
	void Multicast_PlayAnimMontage(UAnimMontage* AnimMontage);
	void Multicast_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UMeleeAttackComponent* MeleeAttackComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	URangeAttackComponent* RangeAttackComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On Death")
	float LifeSpanOnDeath = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On Death")
	bool MeleeCharacter = true;

	UFUNCTION()
	void OnDeath();
};
