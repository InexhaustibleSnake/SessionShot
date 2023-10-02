// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Animations/Notify/AttackStartNotify.h"

void UAttackStartNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast(true);
	Super::Notify(MeshComp, Animation, EventReference);
}
