// Fill out your copyright notice in the Description page of Project Settings.


#include "P2DGameMode.h"

#include "P2DCharacter.h"
#include "P2DPlayerController.h"
#include "P2DPlayerState.h"

void AP2DGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerClass = AP2DPlayerController::StaticClass();
	PlayerStateClass = AP2DPlayerState::StaticClass();
}

AP2DGameMode* AP2DGameMode::GetInstance(const UObject* WorldContextObject)
{
	if (!GEngine || !WorldContextObject)
	{
		return nullptr;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return Cast<AP2DGameMode>(World->GetAuthGameMode());
	}

	return nullptr;
}

AP2DPlayerController* AP2DGameMode::GetPlayerController(const UObject* WorldContextObject)
{
	if (!GEngine || !WorldContextObject)
	{
		return nullptr;
	}
	
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return Cast<AP2DPlayerController>(World->GetFirstPlayerController());
	}

	return nullptr;
}

APawn* AP2DGameMode::GetPlayerPawn(const UObject* WorldContextObject)
{
	if (!GEngine || !WorldContextObject)
	{
		return nullptr;
	}
	
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World->GetFirstPlayerController()->GetPawn();
	}

	return nullptr;
}

AP2DCharacter* AP2DGameMode::GetPlayerCharacter(const UObject* WorldContextObject)
{
	if (!GEngine || !WorldContextObject)
	{
		return nullptr;
	}
	
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return Cast<AP2DCharacter>(GetPlayerController(World)->GetPawn());
	}

	return nullptr;
}

void AP2DGameMode::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AP2DCharacter* Player = Cast<AP2DCharacter>(VictimActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Killed"));
	}
}

void AP2DGameMode::RegisterEnemy(AActor* Enemy)
{
	Enemies.AddUnique(Enemy);
}

void AP2DGameMode::UnregisterEnemy(AActor* Enemy)
{
	Enemies.RemoveSwap(Enemy);
}
