// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConcentrationChanged, float, NewConcentration);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SESSIONSHOT_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Concentration")
	FOnConcentrationChanged OnConcentrationChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Concentration", ReplicatedUsing = OnRep_Concentration)
	FGameplayAttributeData Concentration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Concentration)

	UPROPERTY(BlueprintReadOnly, Category = "Concentration", ReplicatedUsing = OnRep_MaxConcentration)
	FGameplayAttributeData MaxConcentration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxConcentration)
	
protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_Concentration(const FGameplayAttributeData& OldConcentration);

	UFUNCTION()
	virtual void OnRep_MaxConcentration(const FGameplayAttributeData& OldConcentration);

};
