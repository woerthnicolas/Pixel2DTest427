// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/P2DAttributeComponent.h"

#include "P2DGameMode.h"

// static TAutoConsoleVariable<float> CVarDamageMultiplier(
// 	TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);
//

UP2DAttributeComponent::UP2DAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;

	Energy = 100;
	EnergyMax = Energy;

	EnergyRefillAmount = 10.0f;
}


bool UP2DAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}


bool UP2DAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool UP2DAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

bool UP2DAttributeComponent::IsFullEnergy() const
{
	return Energy == EnergyMax;
}


float UP2DAttributeComponent::GetHealth() const
{
	return Health;
}

float UP2DAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float UP2DAttributeComponent::GetEnergy() const
{
	return Energy;
}

float UP2DAttributeComponent::GetEnergyMax() const
{
	return EnergyMax;
}

bool UP2DAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;


	Health = NewHealth;

	if (ActualDelta != 0.0f)
	{
		OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, ActualDelta);
	}

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		if (AP2DGameMode* GM = AP2DGameMode::GetInstance(GetWorld()))
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	ToggleInvulnerable(true);
	
	FTimerHandle InvulnerableTimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "ToggleInvulnerable", false);
	GetWorld()->GetTimerManager().SetTimer(InvulnerableTimerHandle, TimerDelegate, 2.0f, false);

	return ActualDelta != 0;
}

bool UP2DAttributeComponent::ApplyEnergyChange(float Delta)
{
	if (FMath::Abs(Delta) <= KINDA_SMALL_NUMBER)
	{
		return false;
	}

	float OldEnergy = Energy;
	float NewEnergy = FMath::Clamp(Energy + Delta, 0.0f, EnergyMax);

	float ActualDelta = NewEnergy - OldEnergy;


	Energy = NewEnergy;

	if (ActualDelta != 0.0f)
	{
		OnEnergyChanged.Broadcast(nullptr, this, NewEnergy, ActualDelta);
	}

	if(NewEnergy < EnergyMax)
	{
		GetWorld()->GetTimerManager().SetTimer(EnergyRefillTimerHandle, this, &UP2DAttributeComponent::EnergyRefill, 2.0f, true, 3.0f);
	}

	return ActualDelta != 0;
}

void UP2DAttributeComponent::ToggleInvulnerable(bool Value)
{
	IsInvulnerable = Value;
	UE_LOG(LogTemp, Warning, TEXT("ToggleInvulnerable %s"), Value ? TEXT("true") : TEXT("false"));
}

void UP2DAttributeComponent::EnergyRefill()
{
	Energy = FMath::Clamp(Energy + EnergyRefillAmount, 0.0f, EnergyMax);
	OnEnergyChanged.Broadcast(nullptr, this, Energy, 0.0f);
	if(IsFullEnergy() && EnergyRefillTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(EnergyRefillTimerHandle);
	}
}

UP2DAttributeComponent* UP2DAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UP2DAttributeComponent>(FromActor->GetComponentByClass(UP2DAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UP2DAttributeComponent::IsActorAlive(AActor* Actor)
{
	UP2DAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}