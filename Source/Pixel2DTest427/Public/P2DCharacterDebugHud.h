// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugHud.h"
#include "P2DCharacterDebugHud.generated.h"

/**
 * 
 */
UCLASS()
class PIXEL2DTEST427_API AP2DCharacterDebugHud : public ADebugHUD
{
	GENERATED_BODY()


	virtual void DrawHUD() override;
};
