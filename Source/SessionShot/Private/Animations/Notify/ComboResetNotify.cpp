// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Animations/Notify/ComboResetNotify.h"

void UComboResetNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotifyBroadcast.Broadcast();

	Super::Notify(MeshComp, Animation, EventReference);
}
