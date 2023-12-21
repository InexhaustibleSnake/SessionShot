// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "Components/TimelineComponent.h"
#include "BaseCharacter.generated.h"

class UMeleeAttackComponent;
class URangeAttackComponent;
class UHealthComponent;
class UCameraComponent;
class USpringArmComponent;
class UAbilityComponent;
class UBaseAbility;
class UBaseAttributeSet;
class UAttributeSet;
class UGameplayEffect;
class UCurveFloat;

UCLASS()
class SESSIONSHOT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ABaseCharacter();

    void AddMovement(const FInputActionValue& Value);

    virtual void Attack();

    UFUNCTION()
    void OnPlayerAiming();

    UFUNCTION(BlueprintCallable, Category = "Aim")
    bool IsAiming() const { return Aiming; }

    UFUNCTION(NetMulticast, UnReliable, Category = "Animations")
    void Multicast_PlayAnimMontage(UAnimMontage* AnimMontage);
    void Multicast_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    UBaseAttributeSet* GetAttributeSetBase() const;

    int32 GetCharacterLevel() const { return 1; }
    int32 GetAbilityLevel() const { return 1; }

protected:
    UFUNCTION()
    virtual void BeginPlay() override;
    UFUNCTION()
    virtual void Tick(float DeltaTime) override;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PostInitializeComponents() override;

    virtual void InitializeAttributes();

    virtual void PossessedBy(AController* NewController) override;

    UFUNCTION(Server, Reliable, Category = "Aiming")
    void ServerOnPlayerAiming();
    void ServerOnPlayerAiming_Implementation();

    UFUNCTION()
    void OnRep_Aiming();

    virtual void GiveAbilities();

    UFUNCTION()
    void ChangeSpringArmLength(float Alpha);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UMeleeAttackComponent> MeleeAttackComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<URangeAttackComponent> RangeAttackComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UAbilityComponent> AbilityComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UHealthComponent> HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UCameraComponent> MainCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    TObjectPtr<UBaseAttributeSet> AttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
    TArray<TSubclassOf<UBaseAbility>> DefaultAbilities;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float DefaultSpeed = 600.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float WalkingSpeed = 300;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LifeSpan")
    float LifeSpanOnDeath = 10.0f;

    UPROPERTY(ReplicatedUsing = OnRep_Aiming, BlueprintReadOnly, Category = "Aim")
    bool Aiming = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
    float AimSpringArmLenght = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
    float NonAimSpringArmLenght = 350.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
    float AimingTime = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
    UCurveFloat* AimingCurve = nullptr;

private:
    FTimeline AimingTimeline;

};
