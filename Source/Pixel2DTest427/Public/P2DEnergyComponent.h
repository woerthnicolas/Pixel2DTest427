// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Components/ActorComponent.h"
#include "P2DEnergyComponent.generated.h"

class UParticleSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEnergyChanged, int32, NewValue, EStateChange, Change, AActor*, Actor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxEnergyChanged, int32, NewValue, EStateChange, Change);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PIXEL2DTEST427_API UP2DEnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UP2DEnergyComponent();

protected:
	// Properties

	UPROPERTY(EditAnywhere, Category = Energy)
	int32 StartingEnergy;

	UPROPERTY(EditAnywhere, Category = Energy)
	int32 MaxEnergy;

	UPROPERTY(VisibleAnywhere, Category = Energy)
	int32 CurrentEnergy;

	UPROPERTY(EditAnywhere, Category = FX)
	UParticleSystem* HitEffect;

	// Functions

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnEnergyChanged OnEnergyChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxEnergyChanged OnMaxEnergyChanged;

	UFUNCTION(BlueprintCallable)
	int32 GetEnergy() { return CurrentEnergy; };

	UFUNCTION(BlueprintCallable)
	int32 GetEnergyCap() { return MaxEnergy; };

	UFUNCTION(BlueprintCallable)
	bool DecreaseEnergy(int32 Amount, AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	void IncreaseEnergy(int32 Amount, bool bGoesAboveMax, AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	void IncreaseEnergyCap(int32 Amount, bool bIncreaseCurrentEnergy);

	UFUNCTION(BlueprintCallable)
	void DecreaseEnergyCap(int32 Amount, bool bDecreaseCurrentEnergy);

private:
	UFUNCTION(BlueprintCallable)
	void SetEnergy(int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetEnergyCap(int32 Value);
};
