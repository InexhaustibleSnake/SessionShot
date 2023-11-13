// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attributes/BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetConcentrationAttribute()) 
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxConcentration());
		OnConcentrationChanged.Broadcast(GetConcentration());
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

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UBaseAttributeSet, Concentration, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UBaseAttributeSet, MaxConcentration, COND_OwnerOnly);
}
