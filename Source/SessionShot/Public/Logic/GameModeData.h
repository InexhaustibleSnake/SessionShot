#pragma once

#include "GameModeData.generated.h"

UENUM(BlueprintType)
enum class ETeamsType : uint8
{
    Digamma,
    Heta
};

USTRUCT(BlueprintType)
struct FGameModeMatchTimeData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameModeData", meta = (ClampMin = "60", ClampMax = "1000"))
    float WaitingToStartTime = 10.0f;  // Actors are ticking, but the match has not yet started

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameModeData", meta = (ClampMin = "60", ClampMax = "10000"))
    float InProgressTime = 360.0f;  // Main gameplay

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameModeData", meta = (ClampMin = "60", ClampMax = "10000"))
    float WaitingPostMatchTime = 360.0f;  // Match has ended
};

USTRUCT(BlueprintType)
struct FGameModeData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameModeData")
    FGameModeMatchTimeData GameModeMatchTimeData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameModeData", meta = (ClampMin = "1", ClampMax = "3600"))
    int32 TimeToRespawn = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameModeData")
    TArray<ETeamsType>TeamsInGame{ETeamsType::Digamma, ETeamsType::Heta};
};

