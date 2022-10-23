#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Grabber.h"
#include "Components/CapsuleComponent.h"

ATank::ATank() 
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(BaseMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAxis(TEXT("MoveSides"), this, &ATank::MoveSides);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATank::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction(); // llama a la base!
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void ATank::SetTankGrabber(UGrabber* grabber)
{
	TankGrabber = grabber;
}

void ATank::SetTankThrower(UThrower* thrower)
{
	TankThrower = thrower;
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
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
		CapsuleComp->SetSimulatePhysics(false);
	}

	else if (bCanJump && bHadJumped)
	{
		GetWorldTimerManager().SetTimer(TimeToRotate, this, &ATank::CanRotateCam, Duration, false);
	}

	else if (!bCanJump)
	{
		CapsuleComp->SetSimulatePhysics(true);
	}


	UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (bCanJump ? TEXT("true") : TEXT("false")));
}

void ATank::CanRotateCam()
{
	bHadJumped = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();        
	TankPlayerController = Cast<APlayerController>(GetController());	
}


void ATank::Move(float Value)
{
	FVector DeltaLocation(0.f);

	DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::MoveSides(float Value)
{
	FVector DeltaLocation(0.f);

	DeltaLocation.Y = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);

}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;

	// Yaw = Value * DeltaTime * TurnRate
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

	AddActorLocalRotation(DeltaRotation, true);
	
}

void ATank::Jump()
{
	if(bCanJump)
	{
		CapsuleComp->SetSimulatePhysics(true);
		bHadJumped = true;

		FVector ImpulseDirection = FVector(0, 0, 1);
   		CapsuleComp->AddImpulse(ImpulseDirection * 50000);

		GetWorldTimerManager().SetTimer(StopTheJump, this, &ATank::StopJump, StopTime, false);
	}
}

void ATank::StopJump()
{
 	bHadJumped = false;
}


bool ATank::CanJump(FHitResult& OutHitResult) const
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

