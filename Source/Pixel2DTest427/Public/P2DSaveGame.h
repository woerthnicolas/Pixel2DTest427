// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "P2DSaveGame.generated.h"


USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	/* Identifier for which Actor this belongs to */
	UPROPERTY()
	FName ActorName;

	/* For movable Actors, keep location,rotation,scale. */
	UPROPERTY()
	FTransform Transform;

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:

	/* Player Id defined by the online sub system (such as Steam) converted to FString for simplicity  */ 
	UPROPERTY()
	FString PlayerID;

	// Attribute Component
	UPROPERTY()
	float Health;

	UPROPERTY()
	float HealthMax;

	UPROPERTY()
	float Energy;

	UPROPERTY()
	float EnergyMax;
	
	/* Location if player was alive during save */
	UPROPERTY()
	FVector Location;

	/* Orientation if player was alive during save */ 
	UPROPERTY()
	FRotator Rotation;

	/* We don't always want to restore location, and may just resume player at specific respawn point in world. */
	UPROPERTY()
	bool bResumeAtTransform;
};


/**
 * 
 */
UCLASS()
class PIXEL2DTEST427_API UP2DSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TArray<FPlayerSaveData> SavedPlayers;

	/* Actors stored from a level (currently does not support a specific level and just assumes the demo map) */
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

	FPlayerSaveData* GetPlayerData(APlayerState* PlayerState);
};
