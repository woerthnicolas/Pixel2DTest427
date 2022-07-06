// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/P2DAction.h"
#include "Abilities/P2DActionComponent.h"
// #include "../ActionRoguelike.h"



void UP2DAction::Initialize(UP2DActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


bool UP2DAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UP2DActionComponent* Comp = GetOwningComponent();
	
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}


void UP2DAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UP2DActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	
	TimeStarted = GetWorld()->TimeSeconds;

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}


void UP2DAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	UP2DActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	
	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}


UWorld* UP2DAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}


UP2DActionComponent* UP2DAction::GetOwningComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(USActionComponent::StaticClass());

	return ActionComp;
}

bool UP2DAction::IsRunning() const
{
	return bIsRunning;
}