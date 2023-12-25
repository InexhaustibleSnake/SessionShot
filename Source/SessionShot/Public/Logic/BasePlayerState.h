// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Logic/GameModeData.h"
#include "BasePlayerState.generated.h"

UCLASS()
class SESSIONSHOT_API ABasePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    void AddKills() { ++KillsNum; }

    UFUNCTION(BlueprintCallable, Category = "Player Data")
    int32 GetKills() { return KillsNum; }

    void AddDeaths() { ++DeathsNum; }

    UFUNCTION(BlueprintCallable, Category = "Player Data")
    int32 GetDeaths() { return DeathsNum; }

    void SetTeamType(ETeamsType NewTeamType) { TeamType = NewTeamType; }
    ETeamsType GetTeamType() const { return TeamType; }

protected:
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

    UPROPERTY(Transient, Replicated, BlueprintReadWrite, Category = "Player Data")
    int32 KillsNum = 0;

    UPROPERTY(Transient, Replicated, BlueprintReadWrite, Category = "Player Data")
    int32 DeathsNum = 0;

    UPROPERTY(Transient, Replicated, BlueprintReadWrite, Category = "Player Data")
    ETeamsType TeamType = ETeamsType::Digamma;
};
