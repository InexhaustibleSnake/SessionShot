// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include <Animations/AnimUtils.h>
#include "AttackStateChangeNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttackStateChanged, USkeletalMeshComponent*, EAttackStateTypes);

UCLASS()
class SESSIONSHOT_API UAttackStateChangeNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnAttackStateChanged OnNotifyBroadcast;

	UPROPERTY(EditAnywhere)
	EAttackStateTypes AttackState;
};
