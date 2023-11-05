// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Animations/Notify/PlayCameraShakeNotify.h"

void UPlayCameraShakeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnBroadcast.Broadcast(MeshComp, ShakeStrength);

	Super::Notify(MeshComp, Animation, EventReference);
}
