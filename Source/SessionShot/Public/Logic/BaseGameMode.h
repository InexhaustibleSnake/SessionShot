// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

UCLASS()
class SESSIONSHOT_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void CharacterKilled(AController* VictimController, AController* KillerController);

protected:
	void ResetPlayer();

};
