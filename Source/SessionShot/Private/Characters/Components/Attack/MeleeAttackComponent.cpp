// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attack/MeleeAttackComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Animations/Notify/AttackStartNotify.h"
#include "Characters/Animations/Notify/AttackEndNotify.h"

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

	MakeDamageTrace(TraceStart, TraceEnd, 5.0f);
}

void UMeleeAttackComponent::Server_Attack_Implementation()
{
	if (ComboAttackMap.IsEmpty()) return;

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

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Cyan, false, 5);

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.GetActor())
	{
		HitResult.GetActor()->TakeDamage(Damage, FDamageEvent{}, nullptr, GetOwner());
	}
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
		if (!AttackMontage)
		{
			continue;
		}

		const auto NotifyEvents = ComboAttackMap.Find(Index)->Montage->Notifies;

		for (auto NotifyEvent : NotifyEvents)
		{
			auto AttackStartNotify = Cast<UAttackStartNotify>(NotifyEvent.Notify);
			if (AttackStartNotify)
			{
				AttackStartNotify->OnNotified.AddUObject(this, &UMeleeAttackComponent::OnStateChanged);
				continue;
			}
		}

		for (auto NotifyEvent : NotifyEvents)
		{
			auto AttackEndNotify = Cast<UAttackEndNotify>(NotifyEvent.Notify);
			if (AttackEndNotify)
			{
				AttackEndNotify->OnNotified.AddUObject(this, &UMeleeAttackComponent::OnStateChanged);
				continue;
			}
		}
	}
}

void UMeleeAttackComponent::OnStateChanged(bool Attacks)
{
	IsAttacks = Attacks;
}
