// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ComboResetNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifyBroadcast);

UCLASS()
class SESSIONSHOT_API UComboResetNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnNotifyBroadcast OnNotifyBroadcast;
};
