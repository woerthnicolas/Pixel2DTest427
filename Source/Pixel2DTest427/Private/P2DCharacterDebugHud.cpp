// Fill out your copyright notice in the Description page of Project Settings.


#include "P2DCharacterDebugHud.h"

#include "P2DCharacter.h"
#include "P2DGameMode.h"
#include "P2DPixelCharacterMovement.h"

void AP2DCharacterDebugHud::DrawHUD()
{
	Super::DrawHUD();

	HorizontalOffset = 200.0f;

	if(AP2DGameMode* GM = AP2DGameMode::GetInstance(GetWorld()))
	{
		AP2DCharacter* Character = GM->GetPlayerCharacter(GetWorld());

		AddBool(TEXT("bIsJumping"), Character->bIsJumping);
		AddBool(TEXT("bIsMovingRight"), Character->bIsMovingRight);
		AddBool(TEXT("bIsMoving"), Character->bIsMoving);
		AddBool(TEXT("bIsFalling"), Character->bIsFalling);
		AddBool(TEXT("bIsCrouching"), Character->bIsCrouching);
		AddBool(TEXT("bAttack1"), Character->bAttack1);
		AddBool(TEXT("bAttack2"), Character->bAttack2);
		AddBool(TEXT("bIsAttacking"), Character->bIsAttacking);
		AddBool(TEXT("bCrouchWenAble"), Character->bCrouchWenAble);
		AddBool(TEXT("bIsDownThrust"), Character->bIsDownThrust);
		AddBool(TEXT("bIsSliding"), Character->bIsSliding);
		AddBool(TEXT("bIsWallSliding"), Character->bIsWallSliding);
		AddBool(TEXT("bIsWallClimbing"), Character->bIsWallClimbing);
		AddBool(TEXT("bPressedJump"), Character->bPressedJump);
		AddBool(TEXT("GetCanJump"), Character->PixelCharacterMovement->GetCanJump());
	
		AddFloat(TEXT("CrouchMoveSpeed"), Character->CrouchMoveSpeed);
	
		AddInt(TEXT("bWasJumping"), Character->bWasJumping);
		AddInt(TEXT("JumpCurrentCount"), Character->JumpCurrentCount);
	}
}
