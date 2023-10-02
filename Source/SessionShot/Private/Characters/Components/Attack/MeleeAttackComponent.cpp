// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attack/MeleeAttackComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Animations/Notify/AttackStateChangeNotify.h"
#include "Characters/Animations/Notify/ComboResetNotify.h"
#include "Characters/Animations/Notify/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogMeleeAttackComponent, All, All);

UMeleeAttackComponent::UMeleeAttackComponent()
{
	SetComponentTickEnabled(true);
}

void UMeleeAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());

	InitAnimations();
}

void UMeleeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OwnerCharacter) return;
	
	FVector TraceStart = GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceStartSocket);
	FVector TraceEnd = GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceEndSocket);

	MakeDamageTrace(TraceStart, TraceEnd, CurrentAttackData.Damage);
}

void UMeleeAttackComponent::Server_Attack_Implementation()
{
	if (ComboAttackMap.IsEmpty())
	{
		UE_LOG(LogMeleeAttackComponent, Display, TEXT("MeleeAttackComponent should contain data in ComboAttackMap"));
		return;
	}

	int32 MaxAttackIndex = ComboAttackMap.Num();

	CurrentAttackIndex = ++CurrentAttackIndex % MaxAttackIndex;

	CurrentAttackData = ComboAttackMap[CurrentAttackIndex];

	auto AttackMontage = CurrentAttackData.Montage;

	if (!OwnerCharacter || !AttackMontage) return;

	OwnerCharacter->Multicast_PlayAnimMontage(AttackMontage);
}

void UMeleeAttackComponent::MakeDamageTrace(const FVector TraceStart, const FVector TraceEnd, float Damage)
{
	if (!IsAttacks) return;

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceStartSocket), GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceEndSocket), ECollisionChannel::ECC_Visibility, CollisionParams);
	DrawDebugLine(GetWorld(), GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceStartSocket), GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceEndSocket), FColor::Red, false, 2.0f);

	ApplyDamageToActor(HitResult);
}

void UMeleeAttackComponent::ApplyDamageToActor_Implementation(const FHitResult& HitResult)
{
	if (!HitResult.GetActor()) return;
	
	HitResult.GetActor()->TakeDamage(CurrentAttackData.Damage, FDamageEvent{}, nullptr, GetOwner());

	IsAttacks = false;
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
		
		const auto NotifyEvents = ComboAttackMap.Find(Index)->Montage->Notifies;

		for (auto NotifyEvent : NotifyEvents)
		{
			auto AttackStateNotify = Cast<UAttackStateChangeNotify>(NotifyEvent.Notify);
			if (AttackStateNotify)
			{
				AttackStateNotify->OnNotifyBroadcast.AddUObject(this, &UMeleeAttackComponent::OnStateChanged);
				continue;
			}
		}

		for (auto NotifyEvent : NotifyEvents)
		{
			auto ComboResetNotify = Cast<UComboResetNotify>(NotifyEvent.Notify);
			if (ComboResetNotify)
			{
				ComboResetNotify->OnNotifyBroadcast.AddUObject(this, &UMeleeAttackComponent::ResetCurrentAttackIndex);
				continue;
			}
		}
	}
}

void UMeleeAttackComponent::OnStateChanged(EAttackStateTypes StateType)
{
	IsAttacks = StateType == EAttackStateTypes::AttackStart;
}
