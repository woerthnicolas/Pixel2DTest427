// Fill out your copyright notice in the Description page of Project Settings.


#include "P2DInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/P2DGameplayInterface.h"

UP2DInteractionComponent::UP2DInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP2DInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UP2DInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP2DInteractionComponent::PrimaryInteract(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	IP2DGameplayInterface::Execute_Interact(InFocus, MyPawn);
}