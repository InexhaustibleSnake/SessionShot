// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Logic/GameModeData.h"
#include "SessionShotPlayerStart.generated.h"

UCLASS()
class SESSIONSHOT_API ASessionShotPlayerStart : public APlayerStart
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "TeamData")
    ETeamsType GetTeamType() const { return PlayerStartTeamType; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamData")
    ETeamsType PlayerStartTeamType = ETeamsType::Digamma;
};
