// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/Attack/MeleeAttackComponent.h"
#include "Characters/BaseCharacter.h"
#include "Animations/Notify/AttackStateChangeNotify.h"
#include "Animations/Notify/ComboResetNotify.h"
#include "Animations/AnimUtils.h"
#include "Net/UnrealNetwork.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogMeleeAttackComponent, All, All);

void UMeleeAttackComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<ABaseCharacter>(GetOwner());

    if (GetOwner()->HasAuthority())
    {
        InitAnimations();
    }
}

void UMeleeAttackComponent::Attack()
{
    Super::Attack();

    if (!GetOwner()->HasAuthority()) return;

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

void UMeleeAttackComponent::ExecuteSpecificAttack(const FAttackData& AttackData)
{
    if (!GetWorld()) return;

    CurrentAttackData = AttackData;

    GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &UMeleeAttackComponent::MakeDamageTrace, AttackTraceFrequency, true);
}

void UMeleeAttackComponent::MakeDamageTrace()
{
    if (!GetWorld() || !OwnerCharacter || !OwnerCharacter->HasAuthority()) return;

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    FVector TraceStart = GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceStartSocket);
    FVector TraceEnd = GetMeshSocketLocation(OwnerCharacter, CurrentAttackData.TraceEndSocket);

    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackTraceRadius);

    GetWorld()->SweepSingleByChannel(
        HitResult, TraceStart, TraceEnd, FQuat::Identity, ECollisionChannel::ECC_Visibility, CollisionShape, CollisionParams);

    if (!HitResult.bBlockingHit) return;

    ApplyDamageToActor(HitResult);
    OnAttackStateChanged(OwnerCharacter->GetMesh(), EAttackStateTypes::AttackEnd);
}

void UMeleeAttackComponent::ApplyDamageToActor_Implementation(const FHitResult& HitResult)
{
    const auto TargetActor = HitResult.GetActor();
    if (!TargetActor || !CurrentAttackData.AttackEffect) return;

    auto ActorAbilityComponent = TargetActor->FindComponentByClass<UAbilitySystemComponent>();
    if (!ActorAbilityComponent) return;

    FGameplayEffectContextHandle EffectContext = ActorAbilityComponent->MakeEffectContext();
    EffectContext.AddSourceObject(GetOwner());

    FGameplayEffectSpecHandle SpecHandle =
        ActorAbilityComponent->MakeOutgoingSpec(CurrentAttackData.AttackEffect, CurrentAttackData.AttackEffectLevel, EffectContext);

    ActorAbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

FVector UMeleeAttackComponent::GetMeshSocketLocation(const ACharacter* Character, const FName SocketName) const
{
    if (!Character || !Character->GetMesh()) return FVector::ZeroVector;

    return Character->GetMesh()->GetSocketLocation(SocketName);
}

void UMeleeAttackComponent::InitAnimations()
{
    if (!GetOwner()->HasAuthority()) return;

    if (ComboAttackMap.IsEmpty()) return;

    for (int32 Index = 0; Index < ComboAttackMap.Num(); ++Index)
    {
        UAnimMontage* AttackMontage = ComboAttackMap.Find(Index)->Montage;
        if (!AttackMontage) continue;

        for (auto NotifyEvent : AttackMontage->Notifies)
        {
            auto AnimAttackNotify = Cast<UAttackStateChangeNotify>(NotifyEvent.Notify);
            if (AnimAttackNotify)
            {
                AnimAttackNotify->OnNotifyBroadcast.AddUObject(this, &UMeleeAttackComponent::OnAttackStateChanged);
            }
        }

        auto ComboResetNotify = FindNotifyByClass<UComboResetNotify>(AttackMontage);
        if (ComboResetNotify)
        {
            ComboResetNotify->OnNotifyBroadcast.AddUObject(this, &UMeleeAttackComponent::ResetCurrentAttackIndex);
        }
    }
}

void UMeleeAttackComponent::OnAttackStateChanged(USkeletalMeshComponent* MeshComponent, EAttackStateTypes StateType)
{
    if (!GetWorld() || !OwnerCharacter || MeshComponent != OwnerCharacter->GetMesh()) return;

    if (StateType == EAttackStateTypes::AttackStart)
    {
        GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &UMeleeAttackComponent::MakeDamageTrace, AttackTraceFrequency, true);
    }

    if (StateType == EAttackStateTypes::AttackEnd)
    {
        GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
    }
}

void UMeleeAttackComponent::ResetCurrentAttackIndex(USkeletalMeshComponent* MeshComponent)
{
    if (!OwnerCharacter || OwnerCharacter->GetMesh() != MeshComponent) return;

    CurrentAttackIndex = 0;
}

void UMeleeAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UMeleeAttackComponent, CurrentAttackData, COND_OwnerOnly);
}