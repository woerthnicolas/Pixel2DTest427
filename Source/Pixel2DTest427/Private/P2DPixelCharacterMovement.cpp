// Fill out your copyright notice in the Description page of Project Settings.


#include "P2DPixelCharacterMovement.h"

#include "GameplayTagContainer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UP2DPixelCharacterMovement::UP2DPixelCharacterMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UP2DPixelCharacterMovement::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent* CharacterMovementComp = Cast<UCharacterMovementComponent>(
		GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (IsValid(CharacterMovementComp))
	{
		CharacterMovementComponentRef = CharacterMovementComp;
		OwningPawnRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		OwningActorRef = GetOwner();
		OwningCharacterRef = CharacterMovementComponentRef->GetCharacterOwner();
		BaseGravityScale = CharacterMovementComponentRef->GravityScale;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Valid UCharacterMovementComponent detected! This will not work!"));
	}
}

// Called every frame
void UP2DPixelCharacterMovement::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckMovementMode();
}

bool UP2DPixelCharacterMovement::Dash(float DashSpeed, EStartStop StartStop, bool bStopMovement)
{
	if (!IsValid(OwningActorRef) || !IsValid(OwningPawnRef) || !IsValid(CharacterMovementComponentRef)) return false;

	bool bLocalIsDashing = false;

	if (StartStop == EStartStop::Start)
	{
		if (!bLocalIsDashing)
		{
			bLocalIsDashing = true;
			CharacterMovementComponentRef->SetMovementMode(MOVE_Flying);
			CharacterMovementComponentRef->Velocity = OwningActorRef->GetActorForwardVector() * DashSpeed;
			return bLocalIsDashing;
		}
	}
	else
	{
		if (bStopMovement)
		{
			CharacterMovementComponentRef->StopMovementImmediately();
			CharacterMovementComponentRef->SetMovementMode(MOVE_Falling);
			bLocalIsDashing = false;
			return bLocalIsDashing;
		}
	}

	return bLocalIsDashing;
}

void UP2DPixelCharacterMovement::StartCrouch(bool bIsMoving, float CrouchMoveSpeed, float SlideSpeed,
                                             bool& bIsCrouching,
                                             bool& bIsSliding)
{
	if (!IsValid(CharacterMovementComponentRef)) return;

	bIsCrouching = true;

	if (bIsMoving)
	{
		CharacterMovementComponentRef->MaxWalkSpeedCrouched = SlideSpeed;
		bIsSliding = true;
	}
	else
	{
		CharacterMovementComponentRef->MaxWalkSpeedCrouched = CrouchMoveSpeed;
		bIsSliding = false;
	}
}

void UP2DPixelCharacterMovement::StopCrouch(float BaseMoveSpeed, bool& bIsCrouching)
{
	if (!IsValid(CharacterMovementComponentRef)) return;

	CharacterMovementComponentRef->MaxWalkSpeed = BaseMoveSpeed;

	bIsCrouching = false;
}

bool UP2DPixelCharacterMovement::Jump()
{
	if (!IsValid(OwningActorRef) || !IsValid(OwningPawnRef) || !IsValid(CharacterMovementComponentRef)) return false;

	bool bLocalJump;

	if (bIsTouchingSlideWall || bIsTouchingClimbWall)
	{
		WallJump(WallImpactNormal);
		bLocalJump = true;
		return bLocalJump;
	}
	
	if (CharacterMovementComponentRef->IsFalling() && !OwningCharacterRef->CanJump())
	{
		bIsGliding = true;
		bLocalJump = OwningCharacterRef->CanJump();
	}
	else
	{
		OwningCharacterRef->Jump();
		bLocalJump = OwningCharacterRef->CanJump();
	}

	return bLocalJump;
}

void UP2DPixelCharacterMovement::WallJump(const FVector ImpactNormal)
{
	if (!IsValid(OwningActorRef) || !IsValid(OwningPawnRef) || !IsValid(OwningCharacterRef)) return;

	const bool bFacingLeft = ImpactNormal.X > 0.0f;
	const float FacingRotation = bFacingLeft ? 60.0f : -60.0f;
	const float CharacterRotation = bFacingLeft ? 0.0f : 180.0f;
	const FRotator FacingRotator = FRotator(FacingRotation, 0.f, 0.f);

	const FVector LaunchVelocity = (FacingRotator.RotateVector(ImpactNormal) * WallJumpVelocity) - OwningActorRef->
		GetVelocity();

	OwningCharacterRef->LaunchCharacter(LaunchVelocity, false, false);

	const FRotator NewRotation(0.f, 0.f, CharacterRotation);

	OwningPawnRef->GetController()->SetControlRotation(NewRotation);

	bIsTouchingClimbWall = false;
}

FVector UP2DPixelCharacterMovement::Slide(float NewSlideSpeedModifier)
{
	if (!IsValid(OwningActorRef) || !IsValid(CharacterMovementComponentRef)) return FVector::ZeroVector;

	const FVector Velocity = OwningActorRef->GetVelocity();
	const FVector SlideVelocity(Velocity.X, Velocity.Y, Velocity.Z * NewSlideSpeedModifier);
	CharacterMovementComponentRef->Velocity = SlideVelocity;

	return SlideVelocity;
}

void UP2DPixelCharacterMovement::Move(APawn* Pawn, float MoveAxis, bool& bisMoving, bool& bCanTurn)
{
	if (!IsValid(Pawn)) return;

	if (bIsTouchingClimbWall || bIsTouchingSlideWall)
	{
		bisMoving = false;
		bCanTurn = false;
	}
	else
	{
		bCanTurn = true;
		if (MoveAxis != 0.0f)
		{
			bisMoving = true;
			Pawn->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveAxis);
		}
		else
		{
			bisMoving = false;
		}
	}
}

bool UP2DPixelCharacterMovement::StartWallClimb(float Axis)
{
	if (!IsValid(OwningPawnRef) || !IsValid(CharacterMovementComponentRef)) return false;

	if (Axis != 0)
	{
		if (bIsTouchingClimbWall)
		{
			OwningPawnRef->AddMovementInput(FVector(0.0f, 0.0f, 1.0f), Axis, true);
			return false;
		}
		else
		{
			CharacterMovementComponentRef->StopMovementImmediately();
		}
	}
	else
	{
		if (bCanWallClimb)
		{
			CharacterMovementComponentRef->StopMovementImmediately();
		}
	}
	return false;
}

void UP2DPixelCharacterMovement::MoveUpDown(float MoveAxis, bool& bisPressingUp, bool& bisPressingDown)
{
	if (MoveAxis == 0.0f)
	{
		bisPressingUp = false;
		bisPressingDown = false;
	}
	else if (MoveAxis < 0.0f)
	{
		bisPressingUp = true;
		bisPressingDown = false;
	}
	else if (MoveAxis > 0.0f)
	{
		bisPressingUp = false;
		bisPressingDown = true;
	}
}

void UP2DPixelCharacterMovement::StopJumping()
{
	if(!IsValid(OwningCharacterRef)) return;
	
	OwningCharacterRef->StopJumping();
	bIsGliding = false;
}

void UP2DPixelCharacterMovement::Glide(float NewGlideForce)
{
	if (!IsValid(CharacterMovementComponentRef)) return;
	
	CharacterMovementComponentRef->AddImpulse(FVector(0.0f, 0.0f, NewGlideForce), true);
}

void UP2DPixelCharacterMovement::CharacterLanded()
{
	
}

void UP2DPixelCharacterMovement::CheckMovementMode()
{
	if (!IsValid(CharacterMovementComponentRef)) return;

	if(!bCanWallClimb) return;
	
	FVector StartLocation;
	FVector EndLocation;
	float Radius;
	RayCalculator(StartLocation, EndLocation, Radius);

	FHitResult HitResult;
	const FCollisionShape CollisionShape;
	CollisionShape.MakeSphere(Radius);
	const FCollisionQueryParams CollisionQueryParams;
	bool Hit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		CollisionShape,
		CollisionQueryParams
	);
	if (Hit)
	{
		WallImpactNormal = HitResult.ImpactNormal;
		bool bActorHasTagSlide = HitResult.GetActor()->ActorHasTag("wall_slide");
		if (bActorHasTagSlide)
		{
			bIsTouchingClimbWall = false;
			bIsTouchingSlideWall = true;
			SetPawnRotation(HitResult.GetActor());
			Slide(SlideSpeedModifier);
		}
		else
		{
			bool bActorHasTagClimb = HitResult.GetActor()->ActorHasTag("wall_climb");
			if (bActorHasTagClimb)
			{
				bIsTouchingSlideWall = false;
				bIsTouchingClimbWall = true;
				if (bDoOnce)
				{
					SetPawnRotation(HitResult.GetActor());
					CharacterMovementComponentRef->GravityScale = 0.f;
					CharacterMovementComponentRef->Velocity = FVector::ZeroVector;
					CharacterMovementComponentRef->SetMovementMode(MOVE_Flying);
				}
			}
			else
			{
				bIsTouchingSlideWall = false;
				bIsTouchingClimbWall = false;
				CharacterMovementComponentRef->GravityScale = BaseGravityScale;
				CharacterMovementComponentRef->SetMovementMode(MOVE_Falling);
			}
		}
	}
	else
	{
		bIsTouchingSlideWall = false;
		bIsTouchingClimbWall = false;
		CharacterMovementComponentRef->GravityScale = BaseGravityScale;
		//CharacterMovementComponentRef->SetMovementMode(MOVE_Falling);
		bDoOnce = true;
	}
}

void UP2DPixelCharacterMovement::RayCalculator(FVector& StartLocation, FVector& EndLocation, float& Radius) const
{
	if (!IsValid(OwningActorRef)) return;

	const float CharacterZVelocity = OwningActorRef->GetVelocity().Z;

	if (bCanWallClimb && CharacterZVelocity != 0)
	{
		UCapsuleComponent* CapsuleComp = Cast<UCapsuleComponent>(
			OwningActorRef->GetComponentByClass(UCapsuleComponent::StaticClass()));
		if (CapsuleComp)
		{
			const float CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
			const float CapsuleRadius = CapsuleComp->GetScaledCapsuleRadius();
			const FVector ActorLocation = OwningActorRef->GetActorLocation();

			StartLocation = ActorLocation + FVector(0.f, 0.f, (CapsuleHalfHeight / 2.f) - CapsuleRadius);
			EndLocation = ActorLocation + FVector(0.f, 0.f, ((CapsuleHalfHeight / 2.f) - CapsuleRadius) * -1.f);
			Radius = CapsuleRadius + 1.f;
		}
	}
}

void UP2DPixelCharacterMovement::SetPawnRotation(const AActor* Actor) const
{
	if (!IsValid(OwningActorRef) || !IsValid(OwningPawnRef) || !IsValid(Actor)) return;

	float RotationAngle;

	if (OwningActorRef->GetActorLocation().X <= Actor->GetActorLocation().X)
	{
		RotationAngle = 0.f;
	}
	else
	{
		RotationAngle = -180.f;
	}

	OwningPawnRef->GetController()->SetControlRotation(FRotator(0.f, RotationAngle, 0.f));
}

bool UP2DPixelCharacterMovement::GetCanJump()
{
	if(!IsValid(OwningCharacterRef)) return false;

	return OwningCharacterRef->CanJump();
}
