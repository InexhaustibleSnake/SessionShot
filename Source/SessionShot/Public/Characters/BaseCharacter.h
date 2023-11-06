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

	void OnPlayerAiming();

	UFUNCTION(BlueprintCallable, Category = "Aim")
	bool IsAiming() const { return Aiming; }

	UFUNCTION(NetMulticast, UnReliable, Category = "Animations")
	void Multicast_PlayAnimMontage(UAnimMontage* AnimMontage);
	void Multicast_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(Server, Reliable, Category = "Aiming")
	void ServerOnPlayerAiming();
	void ServerOnPlayerAiming_Implementation();

	UFUNCTION()
	void OnRep_Aiming();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float DefaultSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float WalkingSpeed = 300;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LifeSpan")
	float LifeSpanOnDeath = 10.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Aiming, BlueprintReadOnly, Category = "Aim")
	bool Aiming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
	float AimFOV = 35.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
	float NonAimFOV = 90.0f;

	UFUNCTION()
	void OnDeath();

};
