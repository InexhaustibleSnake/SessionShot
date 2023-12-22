// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "Characters/Components/Attributes/BaseAttributeSet.h"
#include "HealthComponent.generated.h"

class UAbilityComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealthPercent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SESSIONSHOT_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    void InitializeWithAbilityComponent(UAbilityComponent* NewAbilityComponent);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return AttributeSet ? AttributeSet->GetHealth() / AttributeSet->GetMaxHealth() : 0.0f; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealth() const { return AttributeSet->GetHealth(); }

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return AttributeSet ? FMath::IsNearlyZero(AttributeSet->GetHealth()) : 0.0f; }

    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;

protected:
    UFUNCTION()
    void OnDeath(AController* KillerController);

    void OnHealthChangedAttribute(const FOnAttributeChangeData& ChangeData);

    UPROPERTY()
    TObjectPtr<UAbilityComponent> AbilityComponent;

    UPROPERTY()
    TObjectPtr<const UBaseAttributeSet> AttributeSet;
};
