// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Logic/BaseGameMode.h"

void ABaseGameMode::CharacterKilled(AController* VictimController, AController* KillerController)
{
	if (VictimController && VictimController->GetPawn())
	{
		VictimController->GetPawn()->Reset();
	}

	RestartPlayer(VictimController);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ("RestartPlayer"));
}

void ABaseGameMode::ResetPlayer()
{
	if (!GetWorld()) return;

	for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		//if ();
	}
}
