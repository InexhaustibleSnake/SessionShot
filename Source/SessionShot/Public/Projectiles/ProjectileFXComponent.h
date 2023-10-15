// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESSIONSHOT_API UProjectileFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UProjectileFXComponent();

protected:
	virtual void BeginPlay() override;

};
