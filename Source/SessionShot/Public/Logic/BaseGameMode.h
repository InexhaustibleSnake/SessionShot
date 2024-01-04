// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Logic/GameModeData.h"
#include "BaseGameMode.generated.h"

class APlayerStart;
class ASessionShotPlayerStart;

UCLASS()
class SESSIONSHOT_API ABaseGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    ABaseGameMode();

    void CharacterKilled(AController* VictimController, AController* KillerController);

    AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = TEXT("")) override;

protected:
    virtual void BeginPlay() override;

   bool ReadyToStartMatch_Implementation();

    virtual void HandleMatchHasStarted() override;

    void RequestRespawn(AController* Controller);

    void ResetOnePlayer(AController* Controller);

    void CreateTeams();

    UFUNCTION()
    void SetGameMatchState(FName StateName);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameMode")
    FGameModeData GameModeData;

    UPROPERTY(Transient)
    TArray<ASessionShotPlayerStart*> CachedPlayerStarts;

private:
    FTimerHandle MatchTimer;
    FTimerDelegate TimerDelegateInProgress;

};
