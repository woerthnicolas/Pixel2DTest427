// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P2DPixelCharacterMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PIXEL2DTEST427_API  UP2DPixelCharacterMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UP2DPixelCharacterMovement();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool Dash(float DashSpeed, EStartStop StartStop, bool bStopMovement);

	UFUNCTION(BlueprintCallable)
	void StartCrouch(bool bIsMoving, float CrouchMoveSpeed, float SlideSpeed, bool& bIsCrouching, bool& bIsSliding);

	UFUNCTION(BlueprintCallable)
	void StopCrouch(float BaseMoveSpeed, bool& bIsCrouching);

	UFUNCTION(BlueprintCallable)
	bool Jump();

	UFUNCTION(BlueprintCallable)
	void WallJump(FVector ImpactNormal);

	UFUNCTION(BlueprintCallable)
	FVector Slide(float NewSlideSpeedModifier);
	
	UFUNCTION(BlueprintCallable)
	void Move(APawn* Pawn, float MoveAxis, bool& bisMoving, bool& bCanTurn);

	UFUNCTION(BlueprintCallable)
	bool StartWallClimb(float Axis);

	UFUNCTION(BlueprintCallable)
	void MoveUpDown(float MoveAxis, bool& bisPressingUp, bool& bisPressingDown);

	UFUNCTION(BlueprintCallable)
	void StopJumping();

	UFUNCTION(BlueprintCallable)
	void Glide(float NewGlideForce);

	UFUNCTION(BlueprintCallable)
	void CharacterLanded();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Movement)
	float SlideSpeedModifier = 0.7f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float GlideForce = 750.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float WallJumpVelocity = 300.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	bool bCanWallClimb = true;

	UPROPERTY(EditAnywhere, Category = Movement)
	bool bCanGlide;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MaxGlideDuration = 1.f;
	
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponentRef;

	UPROPERTY()
	APawn* OwningPawnRef;

	UPROPERTY()
	AActor* OwningActorRef;

	UPROPERTY()
	ACharacter* OwningCharacterRef;


	float BaseGravityScale = 0.f;


	bool bIsTouchingSlideWall = false;


	bool bIsTouchingClimbWall = false;


	FVector WallImpactNormal;


	bool bIsGliding = false;

	void CheckMovementMode();

	void RayCalculator(FVector& StartLocation, FVector& EndLocation, float& Radius) const;

	void SetPawnRotation(const AActor* Actor) const;

	bool bDoOnce = true;

	bool GetCanJump();

};
