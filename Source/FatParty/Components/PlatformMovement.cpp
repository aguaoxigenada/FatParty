#include "PlatformMovement.h"

UPlatformMovement::UPlatformMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlatformMovement::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetOwner()->GetActorLocation();
	StartRotation = GetOwner()->GetActorRotation();
	TargetRotation = StartRotation;
}

void UPlatformMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bCanMove)
	{
		MovePlatform(DeltaTime);
		RotatePlatform(DeltaTime);
	}
	
}

void UPlatformMovement::SetShouldMove(bool bShouldMove)
{
	bCanMove = bShouldMove;
}
void UPlatformMovement::MovePlatform(float DeltaTime)
{
	// Chequeo de puntero del por si se decide hacer que el objeto que tiene este componente pueda morir. El puntero desaparece cuando muere.
	if (ShouldPlatformReturn())
	{
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		StartLocation = StartLocation + MoveDirection * MoveDistance;
		GetOwner()->SetActorLocation(StartLocation);
		PlatformVelocity = -PlatformVelocity;
	}
	else
	{
		// Mover el Cubo hacia adelante
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		CurrentLocation = CurrentLocation + (PlatformVelocity * DeltaTime);
		GetOwner()->SetActorLocation(CurrentLocation);
	}
}

void UPlatformMovement::RotatePlatform(float DeltaTime)
{
	if (ShouldRotationReturn())
	{
		if(bStartingOver)
		{
			StartRotation = StartRotation + GetOwner()->GetActorRotation();
		}
		else
		{
			StartRotation = GetOwner()->GetActorRotation();
		}

		GetOwner()->AddActorLocalRotation(StartRotation * DeltaTime);
		RotationVelocity = RotationVelocity * -1;
		bStartingOver = !bStartingOver;
	}

	else 
	{
		FRotator CurrentLocation = GetOwner()->GetActorRotation();
		CurrentLocation.Pitch = (RotationVelocity.X * DeltaTime);
		GetOwner()->AddActorLocalRotation(CurrentLocation);		
	}
}

bool UPlatformMovement::ShouldPlatformReturn() const
{
	return GetDistanceMoved() > MoveDistance;
}

bool UPlatformMovement::ShouldRotationReturn() const
{
	return GetDistanceRotated() >= RotationDistance;
}


float UPlatformMovement::GetDistanceMoved() const
{
	return FVector::Dist(StartLocation, GetOwner()->GetActorLocation());
}

float UPlatformMovement::GetDistanceRotated() const
{
	return abs(StartRotation.Pitch - GetOwner()->GetActorRotation().Pitch);
}

