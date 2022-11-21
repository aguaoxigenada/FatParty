#include "KnightCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

AKnightCharacter::AKnightCharacter()
{

}

void AKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AKnightCharacter::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AKnightCharacter::Turn);

	PlayerInputComponent->BindAxis(TEXT("MoveSides"), this, &AKnightCharacter::MoveSides);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AKnightCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AKnightCharacter::Fire);
	//PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AKnightCharacter::Jump);
}

void AKnightCharacter::HandleDestruction()
{
	Super::HandleDestruction(); // llama a la base!
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void AKnightCharacter::SetTankGrabber(UGrabber* grabber)
{
	TankGrabber = grabber;
}

void AKnightCharacter::SetTankThrower(UThrower* thrower)
{
	TankThrower = thrower;
}

void AKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(KnightPlayerController)
	{
		FHitResult HitResult;
		KnightPlayerController->GetHitResultUnderCursor(
			ECC_Visibility, 
			false,
			HitResult);

		RotateTurret(HitResult.ImpactPoint);
	
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint,25.f,	12,	FColor::Blue,false,	-1.f);
	}

	
	FHitResult HitResult;
	bCanJump = CanJump(HitResult);

	if (bCanJump && !bHadJumped)
	{
		GetCapsuleComponent()->SetSimulatePhysics(false);
	}

	else if (bCanJump && bHadJumped)
	{
		GetWorldTimerManager().SetTimer(TimeToRotate, this, &AKnightCharacter::CanRotateCam, Duration, false);
	}

	else if (!bCanJump)
	{
		GetCapsuleComponent()->SetSimulatePhysics(true);
	}
	

	// UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (bCanJump ? TEXT("true") : TEXT("false")));
}

void AKnightCharacter::CanRotateCam()
{
	bHadJumped = false;
}

void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();        
	KnightPlayerController = Cast<APlayerController>(GetController());	
}


void AKnightCharacter::Move(float Value)
{
	FVector DeltaLocation(0.f);

	DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void AKnightCharacter::MoveSides(float Value)
{
	FVector DeltaLocation(0.f);

	DeltaLocation.Y = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);

}

void AKnightCharacter::Turn(float Value)
{
	//FRotator DeltaRotation = FRotator::ZeroRotator;
	FRotator DeltaRotation  = FRotator(0.0f, 0.0f, 0.0f);

	// Yaw = Value * DeltaTime * TurnRate
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

	AddActorLocalRotation(DeltaRotation, true);
	
}

void AKnightCharacter::Jump()
{

	if(bCanJump)
	{
		GetCapsuleComponent()->SetSimulatePhysics(true);
		bHadJumped = true;

		FVector ImpulseDirection = FVector(0, 0, 1);
		GetCapsuleComponent()->AddImpulse(ImpulseDirection * 50000);

		GetWorldTimerManager().SetTimer(StopTheJump, this, &AKnightCharacter::StopJump, StopTime, false);
	}
}

void AKnightCharacter::StopJump()
{
 	bHadJumped = false;
}


bool AKnightCharacter::CanJump(FHitResult& OutHitResult) const
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorUpVector() * MaxDistance * -1;

	// Para ver las lineas:
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		Sphere);

	
}

