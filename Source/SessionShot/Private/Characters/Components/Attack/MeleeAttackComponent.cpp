// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attack/MeleeAttackComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Animations/Notify/AttackStateChangeNotify.h"
#include "Characters/Animations/Notify/ComboResetNotify.h"
#include "Characters/Animations/Notify/AnimUtils.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogMeleeAttackComponent, All, All);

void UMeleeAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()) return;

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (!OwnerCharacter) return;

	AttackTimerDelegate.BindUFunction(this, FName("MakeDamageTrace"), CurrentAttackData.TraceStartSocket, CurrentAttackData.TraceEndSocket, CurrentAttackData.Damage);

	InitAnimations();
}

void UMeleeAttackComponent::Server_Attack_Implementation()
{
	if (ComboAttackMap.IsEmpty())
	{
		UE_LOG(LogMeleeAttackComponent, Display, TEXT("MeleeAttackComponent should contain data in ComboAttackMap"));
		return;
	}

	CurrentAttackData = ComboAttackMap[CurrentAttackIndex];

	auto AttackMontage = CurrentAttackData.Montage;

	if (!OwnerCharacter || !AttackMontage) return;

	OwnerCharacter->Multicast_PlayAnimMontage(AttackMontage);

	int32 MaxAttackIndex = ComboAttackMap.Num();

	CurrentAttackIndex = ++CurrentAttackIndex % MaxAttackIndex;
}

void UMeleeAttackComponent::MakeDamageTrace(const FVector TraceStart, const FVector TraceEnd, float Damage)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceStartSocket), GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceEndSocket), ECollisionChannel::ECC_Visibility, CollisionParams);

	DrawDebugLine(GetWorld(), GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceStartSocket), GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceEndSocket), FColor::Red, false, 2.0f);

	ApplyDamageToActor(HitResult);

	if (!HitResult.bBlockingHit) return;
	GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
}

void UMeleeAttackComponent::ApplyDamageToActor_Implementation(const FHitResult& HitResult)
{
	if (!HitResult.GetActor()) return;
	
	HitResult.GetActor()->TakeDamage(CurrentAttackData.Damage, FDamageEvent{}, nullptr, GetOwner());
}

FVector UMeleeAttackComponent::GetMeshSocketLocation(const ACharacter* Character, const FName SocketName) const
{
	if (!Character || !Character->GetMesh()) return FVector::ZeroVector;

	return Character->GetMesh()->GetSocketLocation(SocketName);
}

void UMeleeAttackComponent::InitAnimations()
{
	if (ComboAttackMap.IsEmpty()) return;

	for (int32 Index = 0; Index < ComboAttackMap.Num(); ++Index)
	{
		UAnimMontage* AttackMontage = ComboAttackMap.Find(Index)->Montage;
		if (!AttackMontage)	continue;


		for (int32 i = 0; i < AttackMontage->Notifies.Num(); ++i)
		{
			auto AnimNotifyEvent = FindNotifyByClass<UAttackStateChangeNotify>(AttackMontage);

			AnimNotifyEvent->OnNotifyBroadcast.IsBound();

			if (AnimNotifyEvent)
			{
				AnimNotifyEvent->OnNotifyBroadcast.AddUObject(this, &UMeleeAttackComponent::OnStateChanged);
			}
		}	 
		//for (const auto NotifyEvent : AttackMontage->Notifies)
		//{
		//	const auto AnimNotifyEvent = Cast<UAttackStateChangeNotify>(NotifyEvent.Notify);

		//	if (AnimNotifyEvent)
		//	{
		//		AnimNotifyEvent->OnNotifyBroadcast.AddUObject(this, &UMeleeAttackComponent::OnStateChanged);
		//	}
		//}
		
		auto ComboResetNotify = FindNotifyByClass<UComboResetNotify>(AttackMontage);
		if (ComboResetNotify)
		{
			ComboResetNotify->OnNotifyBroadcast.AddUObject(this, &UMeleeAttackComponent::ResetCurrentAttackIndex);
		}
	}
}

void UMeleeAttackComponent::OnStateChanged(EAttackStateTypes StateType)
{
	if (StateType == EAttackStateTypes::AttackStart)
	{
		if (!GetWorld()) return;
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, AttackTimerDelegate, AttackTraceFrequency, true);
	}

	if (StateType == EAttackStateTypes::AttackEnd)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Null");
	}
}

void UMeleeAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMeleeAttackComponent, CurrentAttackData);
}