// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Logic/BaseGameMode.h"
#include "Logic/BasePlayerState.h"
#include "Logic/GameModeData.h"
#include "Logic/SessionShotPlayerStart.h"
#include "Engine.h"

ABaseGameMode::ABaseGameMode()
{
    bUseSeamlessTravel = true;
}

void ABaseGameMode::BeginPlay()
{
    Super::BeginPlay();
    TimerDelegateInProgress.BindUFunction(this, FName("SetGameMatchState"), FName("InProgress"));

    if (!GetWorld()) return;

    GetWorldTimerManager().SetTimer(MatchTimer, TimerDelegateInProgress, GameModeData.GameModeMatchTimeData.WaitingToStartTime, false);
}

bool ABaseGameMode::ReadyToStartMatch_Implementation()
{
    return GetMatchState() == FName("InProgress");
}

void ABaseGameMode::HandleMatchHasStarted()
{
    CreateTeams();

    Super::HandleMatchHasStarted();
}

void ABaseGameMode::CharacterKilled(AController* VictimController, AController* KillerController)
{
    if (VictimController)
    {
        const auto VictimState = Cast<ABasePlayerState>(VictimController->PlayerState);
        if (VictimState) VictimState->AddDeaths();
    }

    if (KillerController)
    {
        const auto KillerState = Cast<ABasePlayerState>(KillerController->PlayerState);
        if (KillerState) KillerState->AddKills();
    }

    RequestRespawn(VictimController);
}

AActor* ABaseGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
    if (!GetWorld()) return nullptr;

    if (CachedPlayerStarts.IsEmpty())
    {
        for (TActorIterator<ASessionShotPlayerStart> ReceivedPlayerStart(GetWorld()); ReceivedPlayerStart; ++ReceivedPlayerStart)
        {
            CachedPlayerStarts.Add(*ReceivedPlayerStart);
        }
    }

    const auto PlayerController = Cast<APlayerController>(Player);
    if (!PlayerController) return nullptr;

    const auto PlayerState = Cast<ABasePlayerState>(PlayerController->PlayerState);
    if (!PlayerState) return nullptr;

    TArray<ASessionShotPlayerStart*> AvaiblePlayerStarts;

    for (ASessionShotPlayerStart* OnePlayerStart : CachedPlayerStarts)
    {
        if (OnePlayerStart && PlayerState->GetTeamType() == OnePlayerStart->GetTeamType()) AvaiblePlayerStarts.Add(OnePlayerStart);
    }

    if (AvaiblePlayerStarts.IsEmpty()) return nullptr;

    return AvaiblePlayerStarts[FMath::RandRange(0, AvaiblePlayerStarts.Num() - 1)];
}

void ABaseGameMode::RequestRespawn(AController* Controller)
{
    ResetOnePlayer(Controller);
}

void ABaseGameMode::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }

    RestartPlayer(Controller);
}

void ABaseGameMode::CreateTeams()
{
    if (!GetWorld() || GameModeData.TeamsInGame.IsEmpty()) return;

    ETeamsType TeamType = ETeamsType::Digamma;

    for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
    {
        const auto Controller = Iterator->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ABasePlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->SetTeamType(TeamType);

        TeamType = TeamType == ETeamsType::Digamma ? ETeamsType::Heta : ETeamsType::Digamma;
    }
}

void ABaseGameMode::SetGameMatchState(FName StateName)
{
    SetMatchState(StateName);
}