// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConcentrationChanged, float, NewConcentrationPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AController*, KillerController);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                                       \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                                             \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SESSIONSHOT_API UBaseAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Concentration")
    FOnConcentrationChanged OnConcentrationChanged;

    UPROPERTY(BlueprintAssignable, Category = "Health")
    mutable FOnDeath OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Concentration")
    float GetConcentrationPercent() const { return GetConcentration() / GetMaxConcentration(); }

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetIsInvulnerable(bool IsInvulnerable) const { Invulnerable = IsInvulnerable; }

    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Concentration)
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxConcentration)
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingDamage)

protected:
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void SetNewHealth(float NewHealth);
    void SetNewConcentration(float NewConcentration);

    UFUNCTION()
    virtual void OnRep_Concentration(const FGameplayAttributeData& OldConcentration);

    UFUNCTION()
    virtual void OnRep_MaxConcentration(const FGameplayAttributeData& OldConcentration);

    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

    UFUNCTION()
    virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldHealth);

    void ClampAttribute(const FGameplayAttribute Attribute, float NewValue);

    UPROPERTY(BlueprintReadOnly, Category = "Concentration", ReplicatedUsing = OnRep_Concentration)
    FGameplayAttributeData Concentration;

    UPROPERTY(BlueprintReadOnly, Category = "Concentration", ReplicatedUsing = OnRep_MaxConcentration)
    FGameplayAttributeData MaxConcentration;

    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;

    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    mutable bool Invulnerable = false;

    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData IncomingDamage;

};
