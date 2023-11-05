// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlayCameraShakeNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBroadcast, USkeletalMeshComponent*, float);

UCLASS()
class SESSIONSHOT_API UPlayCameraShakeNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnBroadcast OnBroadcast;

protected:
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	float ShakeStrength = 1.0f;
};
