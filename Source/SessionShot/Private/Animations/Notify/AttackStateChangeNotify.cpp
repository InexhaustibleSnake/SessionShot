// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Animations/Notify/AttackStateChangeNotify.h"

void UAttackStateChangeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotifyBroadcast.Broadcast(MeshComp, AttackState);

	Super::Notify(MeshComp, Animation, EventReference);
}
