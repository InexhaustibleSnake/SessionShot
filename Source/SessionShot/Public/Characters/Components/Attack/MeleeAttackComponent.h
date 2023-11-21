// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/Attack/BaseAttackComponent.h"
#include "Animations/AnimUtils.h"
#include "MeleeAttackComponent.generated.h"

class UGameplayEffect;
class ABaseCharacter;

USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    TSubclassOf<UGameplayEffect> AttackEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    int32 AttackEffectLevel = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    UAnimMontage* Montage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    FName TraceStartSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    FName TraceEndSocket;
};

UCLASS()
class SESSIONSHOT_API UMeleeAttackComponent : public UBaseAttackComponent
{
    GENERATED_BODY()

public:
    virtual void Attack() override;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    void ExecuteSpecificAttack(const FAttackData& AttackData);

protected:
    virtual void BeginPlay() override;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

    UFUNCTION()
    void MakeDamageTrace();

    UFUNCTION(Reliable, Server)
    void ApplyDamageToActor(const FHitResult& HitResult);
    void ApplyDamageToActor_Implementation(const FHitResult& HitResult);

    FVector GetMeshSocketLocation(const ACharacter* Character, const FName SocketName) const;

    void InitAnimations();

    void OnAttackStateChanged(USkeletalMeshComponent* MeshComponent, EAttackStateTypes StateType);

    void ResetCurrentAttackIndex(USkeletalMeshComponent* MeshComponent);

    template <typename T>
    T* FindNotifyByClass(UAnimSequenceBase* Animation);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    int32 CurrentAttackIndex = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float AttackTraceFrequency = 0.01f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float AttackTraceRadius = 50.0f;

    UPROPERTY(Replicated)
    FAttackData CurrentAttackData;

    ABaseCharacter* OwnerCharacter;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    TMap<int32, FAttackData> ComboAttackMap;

private:
    FTimerHandle AttackTimer;
};

template <typename T>
inline T* UMeleeAttackComponent::FindNotifyByClass(UAnimSequenceBase* Animation)
{
    if (!Animation) return nullptr;

    const auto NotifyEvents = Animation->Notifies;

    for (auto NotifyEvent : NotifyEvents)
    {
        auto AnimNotify = Cast<T>(NotifyEvent.Notify);
        if (AnimNotify)
        {
            return AnimNotify;
        }
    }

    return nullptr;
}
