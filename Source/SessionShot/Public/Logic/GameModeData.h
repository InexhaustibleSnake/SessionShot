#pragma once

#include "GameModeData.generated.h"

UENUM(BlueprintType)
enum class ETeamsType : uint8
{
    Digamma,
    Heta
};

USTRUCT(BlueprintType)
struct FGameModeData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameModeData", meta = (ClampMin = "60", ClampMax = "3600"))
    float GameMatchTime = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameModeData", meta = (ClampMin = "1", ClampMax = "3600"))
    int32 TimeToRespawn = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameModeData")
    TArray<ETeamsType>TeamsInGame{ETeamsType::Digamma, ETeamsType::Heta};
};
