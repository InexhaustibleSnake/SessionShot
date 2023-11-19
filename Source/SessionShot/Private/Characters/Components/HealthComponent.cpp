// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Logic/BaseGameMode.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Characters/Components/AbilityComponent.h"
#include "Characters/Components/Attributes/BaseAttributeSet.h"
#include "GameplayEffectTypes.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    SetComponentTickEnabled(false);

    SetIsReplicatedByDefault(true);
}

void UHealthComponent::InitializeWithAbilityComponent(UAbilityComponent* NewAbilityComponent)
{
    if (!NewAbilityComponent || NewAbilityComponent == AbilityComponent) return;

    AbilityComponent = NewAbilityComponent;

    AttributeSet = NewAbilityComponent->GetSet<UBaseAttributeSet>();

    if (!AttributeSet) return;

    AbilityComponent->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHealthAttribute())
        .AddUObject(this, &UHealthComponent::OnHealthChangedAttribute);

    OnHealthChanged.Broadcast(GetHealthPercent());
}

void UHealthComponent::ServerKilled_Implementation(AController* KillerController)
{
    if (!GetWorld()) return;

    const auto GameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto OwnerCharacter = Cast<APawn>(GetOwner());
    const auto OwnerController = OwnerCharacter ? OwnerCharacter->GetController() : nullptr;

    GameMode->CharacterKilled(OwnerController, KillerController);
}

void UHealthComponent::OnHealthChangedAttribute(const FOnAttributeChangeData& ChangeData)
{
    float NewHealth = ChangeData.NewValue;

    OnHealthChanged.Broadcast(GetHealthPercent());

    if (FMath::IsNearlyZero(NewHealth))
    {
        ServerKilled(nullptr);
    }
}
