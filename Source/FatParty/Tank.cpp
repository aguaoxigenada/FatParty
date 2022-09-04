// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values
ATank::ATank() // Constructor
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(BaseMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAxis(TEXT("MoveSides"), this, &ATank::MoveSides);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction(); // llama a la base!
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility, 
			false,
			HitResult);

		RotateTurret(HitResult.ImpactPoint);
	/*
		DrawDebugSphere(
			GetWorld(), 
			HitResult.ImpactPoint,
			25.f,
			12,
			FColor::Blue,
			false,
			-1.f);

	*/
	}
}

void ATank::BeginPlay()
{
	Super::BeginPlay();         //tipo de * que se quiere // tipo de pointer actual
	TankPlayerController = Cast<APlayerController>(GetController());	
								// object         // pointer
						  // Retorna APlayerController*  pointer.
	//Casting changes the value of type of one pointer to another pointer type.
}


void ATank::Move(float Value)
{
	//FVector DeltaLocation(0.f); valida tb   
	FVector DeltaLocation = FVector::ZeroVector;
	
	DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);

	//UE_LOG(LogTemp, Warning, TEXT("this is the value: %f"), Value);
}

void ATank::MoveSides(float Value)
{
	FVector DeltaLocation(0.f); 
	
	DeltaLocation.Y = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);

}

void ATank::Turn(float Value)
{
									  // esta es la forma de acceder variables, funciones estaticas
	FRotator DeltaRotation = FRotator::ZeroRotator;
	// Yaw = Value * DeltaTime * TurnRate
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}

// this hace referencia a un puntero dfe este archivo
// :: scope resolution operator
