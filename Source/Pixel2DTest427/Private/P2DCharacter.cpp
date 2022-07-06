// Fill out your copyright notice in the Description page of Project Settings.


#include "P2DCharacter.h"

#include "P2DEnergyComponent.h"
#include "P2DGameMode.h"
#include "P2DInteractionComponent.h"
#include "P2DPixelCharacterMovement.h"
#include "P2DPlayerController.h"
#include "Pixel2DComponent.h"
#include "Abilities/P2DAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/P2DGameplayInterface.h"
#include "Kismet/GameplayStatics.h"

AP2DCharacter::AP2DCharacter()
{
	AttributeComp = CreateDefaultSubobject<UP2DAttributeComponent>(TEXT("AttributeComp"));
	EnergyComp = CreateDefaultSubobject<UP2DEnergyComponent>(TEXT("EnergyComponent"));
	PixelCharacterMovement = CreateDefaultSubobject<UP2DPixelCharacterMovement>(TEXT("PixelCharacterMovement"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnLocation"));
	ArrowComp->SetupAttachment(RootComponent);

	InteractionComp = CreateDefaultSubobject<UP2DInteractionComponent>(TEXT("InteractionComp"));
}

void AP2DCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupCharacter();
}

void AP2DCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckCharacterStatus();
}

void AP2DCharacter::CHEATApplyDamage()
{
	AttributeComp->ApplyHealthChange(nullptr, -10.0f);
}

void AP2DCharacter::CHEATApplyEnergy()
{
	AttributeComp->ApplyEnergyChange(-10.0f);
}


AP2DPlayerController* AP2DCharacter::GetPlayerController()
{
	return Cast<AP2DPlayerController>(GetWorld()->GetFirstPlayerController());
}

APawn* AP2DCharacter::GetPlayerPawn()
{
	return GetWorld()->GetFirstPlayerController()->GetPawn();
}

float AP2DCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	AttributeComp->ApplyHealthChange(DamageCauser, DamageAmount);

	return DamageAmount;
}

void AP2DCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &AP2DCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveDown", this, &AP2DCharacter::MoveDown);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AP2DCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AP2DCharacter::StopJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AP2DCharacter::InteractFunction);
}

void AP2DCharacter::SetupCharacter()
{
	BaseMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if (AP2DPlayerController* PC = GetPlayerController())
	{
		EnableInput(PC);
		PC->SetViewTargetWithBlend(CameraComp->GetOwner(), 0.1f, VTBlend_Linear, 1.0f);
		DefaultMaxJump = JumpMaxCount;
	}

	bCanSlide = true;
}

void AP2DCharacter::CheckCharacterStatus()
{
	bIsWallSliding = PixelCharacterMovement->bIsTouchingSlideWall;
	bIsWallClimbing = PixelCharacterMovement->bIsTouchingClimbWall;
}

void AP2DCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UGameplayStatics::PlaySoundAtLocation(this, LandedSound, GetActorLocation());

	if (bIsDownThrust)
	{
		UGameplayStatics::PlayWorldCameraShake(this, LandCameraShake, GetActorLocation(), 0.0f, 333.0f, 1.0f);
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation());
	}

	bIsDownThrust = false;
	bIsJumping = false;
	bIsFalling = false;
}

void AP2DCharacter::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal,
                                                     const FVector& PreviousFloorContactNormal,
                                                     const FVector& PreviousLocation, float TimeDelta)
{
	bIsFalling = true;
	bIsMoving = false;
	bIsCrouching = false;
	GetCharacterMovement()->MaxWalkSpeed = BaseMoveSpeed;
}

void AP2DCharacter::MoveRight(float X)
{
	bool bCanTurn = true;

	if(APawn* PlayerPawn = GetPlayerPawn())
	{
		PixelCharacterMovement->Move(PlayerPawn, X, bIsMoving, bCanTurn);
		if (bCanTurn)
		{
			FRotator Rotation;
			if (X < 0.0f)
			{
				Rotation = FRotator(0.0f, -180.0f, 0.0f);
				GetController()->SetControlRotation(Rotation);
			}
			else if (X > 0.0f)
			{
				Rotation = FRotator(0.0f, 0.0f, 0.0f);
				GetController()->SetControlRotation(Rotation);
			}
		}
	}
}

void AP2DCharacter::MoveDown(float Y)
{
	PixelCharacterMovement->MoveUpDown(Y, bPressUp, bPressDown);
	if (bIsWallClimbing)
	{
		PixelCharacterMovement->StartWallClimb(Y);
	}
	if (bIsCrouching && bPressDown)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	}
}

void AP2DCharacter::StartJump()
{
	if (bIsCrouching)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AP2DCharacter::ExecuteJump, 0.05f, false);
	}
	else
	{
		ExecuteJump();
	}
}

void AP2DCharacter::StopJump()
{
	PixelCharacterMovement->StopJumping();
}

void AP2DCharacter::ResetJump()
{
	bIsJumping = false;
	bIsFalling = true;
}

void AP2DCharacter::ExecuteJump()
{
	if (PixelCharacterMovement->Jump())
	{
		bIsJumping = true;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AP2DCharacter::ResetJump, 0.1f, false);
	}
}

void AP2DCharacter::InteractFunction()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	UE_LOG(LogTemp, Warning, TEXT("OverlappingActors : %d"), OverlappingActors.Num());
	
	if (InteractionComp)
	{
		for (AActor* OverlapingActor : OverlappingActors)
		{
			if(OverlapingActor->Implements<UP2DGameplayInterface>())
			{
				InteractionComp->PrimaryInteract(OverlapingActor);
			}
		}
	}
}

void AP2DCharacter::OnJumped_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
}
