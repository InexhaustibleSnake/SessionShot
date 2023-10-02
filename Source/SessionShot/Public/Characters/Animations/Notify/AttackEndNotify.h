// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackEndNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDelegateSignature, bool);

UCLASS()
class SESSIONSHOT_API UAttackEndNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnDelegateSignature OnNotified;
};
