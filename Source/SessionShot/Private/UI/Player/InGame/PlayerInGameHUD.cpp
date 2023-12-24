// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "UI/Player/InGame/PlayerInGameHUD.h"
#include "UI/Player/InGame/PlayerHUDWidget.h"

void APlayerInGameHUD::BeginPlay() 
{
    Super::BeginPlay();

    const auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (!PlayerHUDWidget) return;

    PlayerHUDWidget->AddToViewport();
}

