// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/Attack/BaseAttackComponent.h"
#include "MeleeAttackComponent.generated.h"

UCLASS()
class SESSIONSHOT_API UMeleeAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()

protected:
	virtual void Server_Attack_Implementation() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	int32 CurrentAttackIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	TMap<int32, UAnimMontage*> ComboAttackMap;
};
