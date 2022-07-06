// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P2DPowerupActor.h"
#include "P2DPowerup_HealthPotion.generated.h"


class UStaticMeshComponent;


/**
 * 
 */
UCLASS()
class PIXEL2DTEST427_API AP2DPowerup_HealthPotion : public AP2DPowerupActor
{
	GENERATED_BODY()
	

public:
	AP2DPowerup_HealthPotion();
	
	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
