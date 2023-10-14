// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAttackComponent.generated.h"

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SESSIONSHOT_API UBaseAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseAttackComponent();

	virtual void Attack();

protected:
	virtual void BeginPlay() override;
		
	UFUNCTION(Server, Reliable)
	virtual void Server_Attack();
	virtual void Server_Attack_Implementation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	TMap<int32, FAttackData> ComboAttackMap;

};
