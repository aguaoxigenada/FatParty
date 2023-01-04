#include "PlatformMovement.h"

UPlatformMovement::UPlatformMovement()
{

	PrimaryComponentTick.bCanEverTick = true;
}


void UPlatformMovement::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetOwner()->GetActorLocation();
	
}

void UPlatformMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);
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
	GetOwner()->AddActorLocalRotation(RotationVelocity * DeltaTime);
}

bool UPlatformMovement::ShouldPlatformReturn() const
{
	return GetDistanceMoved() > MoveDistance;
}

float UPlatformMovement::GetDistanceMoved() const
{
	return FVector::Dist(StartLocation, GetOwner()->GetActorLocation());
}

