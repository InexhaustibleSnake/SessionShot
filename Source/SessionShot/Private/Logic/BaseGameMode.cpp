// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Logic/BaseGameMode.h"
#include "Logic/BasePlayerState.h"
#include "Logic/GameModeData.h"

void ABaseGameMode::StartPlay()
{
    Super::StartPlay();

    CreateTeams();
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
    if (!GetWorld() || GameModeData.TeamsInGame.IsEmpty());

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
