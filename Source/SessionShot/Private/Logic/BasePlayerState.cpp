// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Logic/BasePlayerState.h"
#include "Net/UnrealNetwork.h"

void ABasePlayerState::SetPlayerCharacterClass(const TSubclassOf<ABaseCharacter> NewPlayerCharacterClass) 
{
    if (!NewPlayerCharacterClass) return;
    PlayerCharacterClass = NewPlayerCharacterClass;
}

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ABasePlayerState, KillsNum);
    DOREPLIFETIME(ABasePlayerState, DeathsNum);
    DOREPLIFETIME(ABasePlayerState, TeamType);
    DOREPLIFETIME(ABasePlayerState, PlayerCharacterClass);
}
