// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/Attack/BaseAttackComponent.h"
#include "Characters/Animations/Notify/AnimUtils.h"
#include "MeleeAttackComponent.generated.h"

class ABaseCharacter;

UCLASS()
class SESSIONSHOT_API UMeleeAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void Server_Attack_Implementation() override;

	UFUNCTION()
	void MakeDamageTrace(const FVector TraceStart, const FVector TraceEnd, float Damage);

	UFUNCTION(Reliable, Server)
	void ApplyDamageToActor(const FHitResult& HitResult);
	void ApplyDamageToActor_Implementation(const FHitResult& HitResult);

	FVector GetMeshSocketLocation(const ACharacter* Character, const FName SocketName) const;

	void InitAnimations();

	void OnStateChanged(EAttackStateTypes StateType);

	void ResetCurrentAttackIndex() { CurrentAttackIndex = 0; }

	template <typename T>
	T* FindNotifyByClass(UAnimSequenceBase* Animation);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	int32 CurrentAttackIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackTraceFrequency = 0.005f;

	UPROPERTY(Replicated)
	FAttackData CurrentAttackData;

	ABaseCharacter* OwnerCharacter;


private:
	FTimerHandle AttackTimer;
	FTimerDelegate AttackTimerDelegate;

};

template<typename T>
inline T* UMeleeAttackComponent::FindNotifyByClass(UAnimSequenceBase* Animation)
{
	if (!Animation) return nullptr;

	const auto NotifyEvents = Animation->Notifies;

	for (auto NotifyEvent : NotifyEvents)
	{
		auto AnimNotify = Cast<T>(NotifyEvent.Notify);
		if (AnimNotify && !AnimNotify->OnNotifyBroadcast.IsBound())
		{
			return AnimNotify;
		}
	}

	return nullptr;
}
