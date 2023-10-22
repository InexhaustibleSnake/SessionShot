// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

class UMeleeAttackComponent;
class URangeAttackComponent;
class UHealthComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SESSIONSHOT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	void AddMovement(const FInputActionValue& Value);

	virtual void Attack();

	virtual void SecondaryAttack();

	void Aim();

	UFUNCTION(BlueprintCallable, Category = "Aim")
	bool IsAiming() const { return Aiming; }

	UFUNCTION(NetMulticast, UnReliable, Category = "Animations")
	void Multicast_PlayAnimMontage(UAnimMontage* AnimMontage);
	void Multicast_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, UnReliable, Category = "Aiming")
	void ServerOnPlayerAiming(bool bAiming);
	void ServerOnPlayerAiming_Implementation(bool bAiming);

	UFUNCTION(NetMulticast, UnReliable, Category = "Aiming")
	void MulticastOnPlayerAiming(bool bAiming);
	void MulticastOnPlayerAiming_Implementation(bool bAiming);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UMeleeAttackComponent* MeleeAttackComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	URangeAttackComponent* RangeAttackComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* MainCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On Death")
	float LifeSpanOnDeath = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool MeleeCharacter = true;

	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	bool Aiming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
	float AimFOV = 35.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
	float NonAimFOV = 90.0f;

	UFUNCTION()
	void OnDeath();
};
