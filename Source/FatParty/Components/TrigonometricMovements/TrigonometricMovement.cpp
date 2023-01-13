#include "TrigonometricMovement.h"

// Sets default values for this component's properties
UTrigonometricMovement::UTrigonometricMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Parent = GetOwner();
	// ...
}


// Called when the game starts
void UTrigonometricMovement::BeginPlay()
{
	Super::BeginPlay();
	StartPosition = Parent->GetActorLocation();
	// ...
	
}


// Called every frame
void UTrigonometricMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Parent)
	{
		float x = MaxMovementDisance.X * FMath::Sin(Frequency.X * RunnningTime) + StartPosition.X;
		float y = MaxMovementDisance.Y * FMath::Sin(Frequency.Y * RunnningTime) + StartPosition.Y;
		float z = MaxMovementDisance.Z * FMath::Sin(Frequency.Z * RunnningTime) + StartPosition.Z;
		Parent->SetActorLocation(FVector(x, y, z));
		RunnningTime += DeltaTime;
	}
	// ...
}

