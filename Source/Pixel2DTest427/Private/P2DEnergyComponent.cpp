// Fill out your copyright notice in the Description page of Project Settings.


#include "P2DEnergyComponent.h"

// Sets default values for this component's properties
UP2DEnergyComponent::UP2DEnergyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UP2DEnergyComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentEnergy = StartingEnergy;
	
}

void UP2DEnergyComponent::SetEnergy(int32 Value)
{
	CurrentEnergy = Value;
}

void UP2DEnergyComponent::SetEnergyCap(int32 Value)
{
	MaxEnergy = Value;
}

bool UP2DEnergyComponent::DecreaseEnergy(int32 Amount, AActor* Instigator)
{
	CurrentEnergy -= Amount;

	OnEnergyChanged.Broadcast(CurrentEnergy, EStateChange::Decreased, Instigator);

	return true;
}

void UP2DEnergyComponent::IncreaseEnergy(int32 Amount, bool bGoesAboveMax, AActor* Instigator)
{
	if(bGoesAboveMax)
	{
		CurrentEnergy += Amount;
	}
	else
	{
		CurrentEnergy = FMath::Min(CurrentEnergy + Amount, MaxEnergy);
	}

	OnMaxEnergyChanged.Broadcast(CurrentEnergy, EStateChange::Increased);
}

void UP2DEnergyComponent::IncreaseEnergyCap(int32 Amount, bool bIncreaseCurrentEnergy)
{
	MaxEnergy += Amount;

	OnMaxEnergyChanged.Broadcast(MaxEnergy, EStateChange::Increased);

	if(bIncreaseCurrentEnergy)
	{
		IncreaseEnergy(Amount, false, nullptr);
	}
}

void UP2DEnergyComponent::DecreaseEnergyCap(int32 Amount, bool bDecreaseCurrentEnergy)
{
	MaxEnergy -= Amount;

	OnMaxEnergyChanged.Broadcast(MaxEnergy, EStateChange::Decreased);

	if(bDecreaseCurrentEnergy && CurrentEnergy > MaxEnergy)
	{
		SetEnergy(MaxEnergy);
	}
}
