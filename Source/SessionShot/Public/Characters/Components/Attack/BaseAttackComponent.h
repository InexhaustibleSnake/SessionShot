// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAttackComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESSIONSHOT_API UBaseAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseAttackComponent();

	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
    void SetCanOwnerAttack(bool CanOwnerAttack) { CanAttack = CanOwnerAttack; }

protected:
	virtual void BeginPlay() override;
		
	UFUNCTION(Server, Reliable)
	virtual void Server_Attack();
	virtual void Server_Attack_Implementation();

	bool CanAttack = true;

};
