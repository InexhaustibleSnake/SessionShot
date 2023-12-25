// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Logic/GameModeData.h"
#include "BaseGameMode.generated.h"

UCLASS()
class SESSIONSHOT_API ABaseGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    virtual void StartPlay() override;

    void CharacterKilled(AController* VictimController, AController* KillerController);

protected:
    void RequestRespawn(AController* Controller);

    void ResetOnePlayer(AController* Controller);

    void CreateTeams();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameMode")
    FGameModeData GameModeData;
};
