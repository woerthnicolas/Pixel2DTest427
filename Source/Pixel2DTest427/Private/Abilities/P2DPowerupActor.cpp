// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/P2DPowerupActor.h"

#include "Components/SphereComponent.h"


AP2DPowerupActor::AP2DPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComp");
	FlipbookComp->SetupAttachment(RootComponent);
	RespawnTime = 0.0f;
	bIsActive = true;
}


void AP2DPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes...
}


FText AP2DPowerupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}


void AP2DPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}


void AP2DPowerupActor::HideAndCooldownPowerup()
{
	if(RespawnTime > 0.0f)
	{
		SetPowerupState(false);

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AP2DPowerupActor::ShowPowerup, RespawnTime);
	}
	else
	{
		SetLifeSpan(0.1f);
	}
}

void AP2DPowerupActor::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	SetActorEnableCollision(bIsActive);
	// Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive, true);
}