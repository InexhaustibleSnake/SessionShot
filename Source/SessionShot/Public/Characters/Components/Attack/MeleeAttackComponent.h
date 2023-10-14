// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/Attack/BaseAttackComponent.h"
#include "Characters/Animations/Notify/AnimUtils.h"
#include "MeleeAttackComponent.generated.h"

class ABaseCharacter;

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float Damage = 10.0f;

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
	UMeleeAttackComponent();

	virtual void Attack() override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	int32 CurrentAttackIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	TMap<int32, FAttackData> ComboAttackMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackTraceFrequency = 0.005f;

	UPROPERTY(Replicated)
	FAttackData CurrentAttackData;

	ABaseCharacter* OwnerCharacter;

private:
	FTimerHandle AttackTimer;
	FTimerDelegate AttackTimerDelegate;

};
