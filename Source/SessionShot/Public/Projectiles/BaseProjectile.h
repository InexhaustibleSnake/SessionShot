// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class SESSIONSHOT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
	virtual void BeginPlay() override;

private:
	FVector ShotDirection;

};
