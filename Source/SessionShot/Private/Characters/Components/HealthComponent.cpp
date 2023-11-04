// The project was made by Alexey Guchmazov Ilich (Inexhaustible Snake) for educational purposes

#include "Characters/Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Logic/BaseGameMode.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()) return;
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);

	Health = MaxHealth;
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsDead() || !DamagedActor) return;

	ServerSetHealth(Health - Damage);

	if (IsDead())
	{
		OnDeath.Broadcast();
		ServerKilled_Implementation(InstigatedBy);
		return;
	}
}

void UHealthComponent::ServerKilled_Implementation(AController* KillerController)
{
	if (!GetWorld()) return;

	const auto GameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	const auto OwnerCharacter = Cast<APawn>(GetOwner());
	const auto OwnerController = OwnerCharacter ? OwnerCharacter->GetController() : nullptr;

	GameMode->CharacterKilled(OwnerController, KillerController);
}

void UHealthComponent::ServerSetHealth_Implementation(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(GetHealthPercent());
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UHealthComponent, Health, COND_OwnerOnly);
}

