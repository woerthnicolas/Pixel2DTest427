// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P2DGameMode.generated.h"

class AP2DPlayerController;
class AP2DCharacter;
/**
 * 
 */
UCLASS()
class AP2DGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, DisplayName = "Get P2D Game Mode", meta = (WorldContext = "WorldContextObject"))
	static AP2DGameMode* GetInstance(const UObject* WorldContextObject);
	static AP2DGameMode& Get(const UObject* WorldContextObject)
	{
		check(IsValid(WorldContextObject));
		auto* Instance = GetInstance(WorldContextObject);
		check(IsValid(Instance));
		return *Instance;
	}
	
	UFUNCTION(BlueprintPure, DisplayName = "Get P2D PlayerController", meta = (WorldContext = "WorldContextObject"))
	AP2DPlayerController* GetPlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, DisplayName = "Get P2D PlayerPawn", meta = (WorldContext = "WorldContextObject"))
	APawn* GetPlayerPawn(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, DisplayName = "Get P2D PlayerCharacter", meta = (WorldContext = "WorldContextObject"))
	AP2DCharacter* GetPlayerCharacter(const UObject* WorldContextObject);

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	void RegisterEnemy(AActor* Enemy);
	void UnregisterEnemy(AActor* Enemy);

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> Enemies;
};
