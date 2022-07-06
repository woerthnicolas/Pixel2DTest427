// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "P2DPlayerController.h"
#include "Pixel2DCharacter.h"
#include "P2DCharacter.generated.h"

class UP2DInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UP2DAttributeComponent;
class UP2DEnergyComponent;
class UP2DPixelCharacterMovement;
class USInteractionComponent;
class UParticleSystem;
/**
 * 
 */
UCLASS()
class PIXEL2DTEST427_API AP2DCharacter : public APixel2DCharacter
{
	GENERATED_BODY()
public:
	AP2DCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(Exec)
	void CHEATApplyDamage();

	UFUNCTION(Exec)
	void CHEATApplyEnergy();

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UP2DAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UP2DEnergyComponent* EnergyComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UP2DPixelCharacterMovement* PixelCharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* ArrowComp;
	
	UPROPERTY(VisibleAnywhere)
	UP2DInteractionComponent* InteractionComp;

#pragma endregion Components

#pragma region Animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsMoving = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsMovingRight = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsJumping = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsCrouching = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bAttack1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bAttack2 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsRangedAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bCrouchWenAble = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bPressUp = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bPressDown = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsDownThrust = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsSliding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsWallSliding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsWallClimbing = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsGrenading = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsHurt = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bPlayerDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	bool bIsRolling = false;

#pragma endregion Animations

#pragma region Character

	UPROPERTY(VisibleAnywhere, Category = Character)
	float CrouchMoveSpeed = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = Character)
	float SlideMoveSpeed = 250.0f;

	UPROPERTY(EditAnywhere, Category = Character)
	float SlideDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category = Character)
	float SlideCoolDown = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = Character)
	int32 DefaultMaxJump = 0;

	UPROPERTY(VisibleAnywhere, Category = Character)
	float RollDuration = 0.7f;

#pragma endregion Character

#pragma region Sounds

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
	USoundBase* HurtSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
	USoundBase* MeleeAttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
	USoundBase* JumpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
	USoundBase* RangedAttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
	USoundBase* LandedSound;

#pragma endregion Sounds

	UPROPERTY(EditAnywhere, Category = Movement)
	float DeadZone = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
	TSubclassOf<UCameraShakeBase> LandCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
	UParticleSystem* HitParticles;
	
	virtual void Landed(const FHitResult& Hit) override;

	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal,
	                                              const FVector& PreviousFloorContactNormal,
	                                              const FVector& PreviousLocation, float TimeDelta) override;
	
	UFUNCTION()
	void MoveRight(float X);
	UFUNCTION()
	void MoveDown(float Y);
	UFUNCTION()
	void StartJump();
	UFUNCTION()
	void StopJump();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	AP2DPlayerController* GetPlayerController();

	UFUNCTION(BlueprintCallable)
	APawn* GetPlayerPawn();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = NotEditable)
	float BaseMoveSpeed = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = NotEditable)
	bool bCanSlide = true;
	
	void SetupCharacter();

	void CheckCharacterStatus();

	UFUNCTION()
	void ResetJump();
	UFUNCTION()
	void ExecuteJump();
	
	UFUNCTION()
	void InteractFunction();

	virtual void OnJumped_Implementation() override;
};
