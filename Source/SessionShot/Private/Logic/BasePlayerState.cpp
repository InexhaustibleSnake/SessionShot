// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Logic/BasePlayerState.h"
#include "Net/UnrealNetwork.h"

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ABasePlayerState, KillsNum);
    DOREPLIFETIME(ABasePlayerState, DeathsNum);
}
