// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UProgressBar;
class UImage;

UCLASS()
class SESSIONSHOT_API UPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "Concentration")
    float GetConcentrationPercent() const;

    UFUNCTION(BlueprintCallable, Category = "Crosshair")
    ESlateVisibility GetCrosshairVisibility() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crosshair")
    bool AlwaysDisplayCrosshair = false;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ConcentrationBar;

    UImage* Crosshair;
};
