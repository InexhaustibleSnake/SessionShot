// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Logic/BaseGameMode.h"
#include "Logic/BasePlayerState.h"

void ABaseGameMode::CharacterKilled(AController* VictimController, AController* KillerController)
{
	const auto VictimState = Cast<ABasePlayerState>(VictimController->PlayerState);
	if (VictimState) VictimState->AddDeaths();

	const auto KillerState = Cast<ABasePlayerState>(VictimController->PlayerState);
	if (KillerState) KillerState->AddKills();

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
