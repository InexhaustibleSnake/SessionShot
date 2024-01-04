// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

UCLASS()
class SESSIONSHOT_API ABaseGameState : public AGameState
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "GameStateData")
    void SetGameMap(const FString& NewMapName) { MapName = NewMapName; }

    UFUNCTION(BlueprintCallable, Category = "GameStateData")
    FString GetGameMap() const { return MapName; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameStateData")
    FString MapName = "";
};
