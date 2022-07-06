// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/P2DActionComponent.h"
// #include "../ActionRoguelike.h"
#include "Abilities/P2DAction.h"

// DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_STANFORD);
//

UP2DActionComponent::UP2DActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void UP2DActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<UP2DAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}


void UP2DActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Stop all
	TArray<UP2DAction*> ActionsCopy = Actions;
	for (UP2DAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UP2DActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw All Actions
	// 	for (USAction* Action : Actions)
	// 	{
	// 		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	// 		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
	// 
	// 		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	// 	}
}


void UP2DActionComponent::AddAction(AActor* Instigator, TSubclassOf<UP2DAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UP2DAction* NewAction = NewObject<UP2DAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}


void UP2DActionComponent::RemoveAction(UP2DAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}


UP2DAction* UP2DActionComponent::GetAction(TSubclassOf<UP2DAction> ActionClass) const
{
	for (UP2DAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}


bool UP2DActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	// SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for (UP2DAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}
			
			// Bookmark for Unreal Insights
			// TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}


bool UP2DActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UP2DAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}