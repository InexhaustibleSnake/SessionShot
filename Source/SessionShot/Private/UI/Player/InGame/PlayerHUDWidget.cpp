// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "UI/Player/InGame/PlayerHUDWidget.h"
#include "Characters/Components/Attributes/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/Image.h"
#include "Characters/BaseCharacter.h"

float UPlayerHUDWidget::GetHealthPercent() const
{
    const auto Pawn = GetOwningPlayerPawn();
    if (!Pawn) return 0.0f;

    const auto AbilityComponent = Pawn->FindComponentByClass<UAbilitySystemComponent>();
    if (!AbilityComponent) return 0.0f;

    const auto AttributeSet = AbilityComponent->GetSet<UBaseAttributeSet>();

    return AttributeSet ? AttributeSet->GetHealthPercent() : 0.0f;
}

float UPlayerHUDWidget::GetConcentrationPercent() const
{
    const auto Pawn = GetOwningPlayerPawn();
    if (!Pawn) return 0.0f;

    const auto AbilityComponent = Pawn->FindComponentByClass<UAbilitySystemComponent>();
    if (!AbilityComponent) return 0.0f;

    const auto AttributeSet = AbilityComponent->GetSet<UBaseAttributeSet>();

    return AttributeSet ? AttributeSet->GetConcentrationPercent() : 0.0f;
}

ESlateVisibility UPlayerHUDWidget::GetCrosshairVisibility() const
{
    if (AlwaysDisplayCrosshair) return ESlateVisibility::Visible;

    const auto BaseCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
    if (!BaseCharacter) return ESlateVisibility::Visible;

    return BaseCharacter->IsAiming() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}
