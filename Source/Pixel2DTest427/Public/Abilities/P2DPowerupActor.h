// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/P2DGameplayInterface.h"
#include "P2DPowerupActor.generated.h"

class UP2DRPGItem;
class USphereComponent;
class UStaticMeshComponent;


UCLASS(ABSTRACT)
class PIXEL2DTEST427_API AP2DPowerupActor : public AActor, public IP2DGameplayInterface
{
	GENERATED_BODY()
public:
	AP2DPowerupActor();
	
	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn);
protected:
	UPROPERTY()
	bool bIsActive;

	// Set a time to make it respawn
	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPaperFlipbookComponent* FlipbookComp;

	UPROPERTY(EditAnywhere, Category = Data)
	UP2DRPGItem* ItemData;
};
