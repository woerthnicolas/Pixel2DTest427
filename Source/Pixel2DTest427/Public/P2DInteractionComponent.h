// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P2DInteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PIXEL2DTEST427_API UP2DInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract(AActor* InFocus);

protected:
	
	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UP2DInteractionComponent();
};
