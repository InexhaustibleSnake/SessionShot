// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attributes/BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    ClampAttribute(Attribute, NewValue);
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) 
{
    Super::PostGameplayEffectExecute(Data);
    
    if (Data.EvaluatedData.Attribute == GetConcentrationAttribute())
    {
        SetNewConcentration(GetConcentration());
        OnConcentrationChanged.Broadcast(GetConcentration());
    }

    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
    {
        SetNewHealth(GetHealth() - GetIncomingDamage());
  
        SetIncomingDamage(0.0f);
    }
}

void UBaseAttributeSet::SetNewHealth(float NewHealth)
{
    SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
}

void UBaseAttributeSet::SetNewConcentration(float NewConcentration)
{
    SetHealth(FMath::Clamp(NewConcentration, 0.0f, GetMaxConcentration()));
}

void UBaseAttributeSet::ClampAttribute(const FGameplayAttribute Attribute, float NewValue)
{
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
}

void UBaseAttributeSet::OnRep_Concentration(const FGameplayAttributeData& OldConcentration)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Concentration, OldConcentration);
}

void UBaseAttributeSet::OnRep_MaxConcentration(const FGameplayAttributeData& OldConcentration)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxConcentration, OldConcentration);
}

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(UBaseAttributeSet, Concentration, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UBaseAttributeSet, MaxConcentration, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UBaseAttributeSet, Health, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UBaseAttributeSet, MaxHealth, COND_OwnerOnly);
}
