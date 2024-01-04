// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attributes/BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Player/MainPlayerController.h"
#include "Logic/BasePlayerState.h"

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
        OnConcentrationChanged.Broadcast(GetConcentrationPercent());
    }

    if (Data.EvaluatedData.Attribute == GetMaxConcentrationAttribute())
    {
        SetMaxConcentration(GetMaxConcentration());
        OnConcentrationChanged.Broadcast(GetConcentrationPercent());
    }

    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
    {
        if (Invulnerable)
        {
            SetIncomingDamage(0.0f);
            return;
        }

        const auto EffectOwnerController = Cast<AController>(Data.EffectSpec.GetEffectContext().GetInstigator());

        if (!AreEnemies(GetOwnerController(), EffectOwnerController))
        {
            SetIncomingDamage(0.0f);
            return;
        }

        SetNewHealth(GetHealth() - GetIncomingDamage());

        SetIncomingDamage(0.0f);
    }

    if (FMath::IsNearlyZero(GetHealth()))
    {
        const auto EffectContext = Data.EffectSpec.GetEffectContext();

        const auto KillerController = Cast<AController>(EffectContext.GetInstigator());

        OnDeath.Broadcast(KillerController);
    }
}

void UBaseAttributeSet::SetNewHealth(float NewHealth)
{
    SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
}

void UBaseAttributeSet::SetNewConcentration(float NewConcentration)
{
    SetConcentration(FMath::Clamp(NewConcentration, 0.0f, GetMaxConcentration()));
    OnRep_Concentration(GetConcentration());
}

void UBaseAttributeSet::ClampAttribute(const FGameplayAttribute Attribute, float NewValue)
{
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }

    if (Attribute == GetConcentrationAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxConcentration());
    }
}

bool UBaseAttributeSet::AreEnemies(const AController* Controller1, const AController* Controller2) const
{
    if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

    const auto PlayerState1 = Cast<ABasePlayerState>(Controller1->PlayerState);
    const auto PlayerState2 = Cast<ABasePlayerState>(Controller2->PlayerState);

    return PlayerState1 && PlayerState2 && PlayerState1->GetTeamType() != PlayerState2->GetTeamType();
}

AController* UBaseAttributeSet::GetOwnerController() const
{
    const auto OwnerPawn = Cast<APawn>(GetOwningActor());
    if (!OwnerPawn) return nullptr;

    return OwnerPawn->GetController();
}

void UBaseAttributeSet::OnRep_Concentration(const FGameplayAttributeData& OldConcentration)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Concentration, OldConcentration);
    OnConcentrationChanged.Broadcast(GetConcentrationPercent());
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
