#include "TrigonometricRotation.h"

UTrigonometricRotation::UTrigonometricRotation()
{
	PrimaryComponentTick.bCanEverTick = true;
	Parent = GetOwner();
}

void UTrigonometricRotation::BeginPlay()
{
	Super::BeginPlay();
	StartRotation = Parent->GetActorRotation();
}

void UTrigonometricRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Parent)
	{
		float x = MaxAngle.X * FMath::Sin(Frequency.X * RunnningTime) + StartRotation.Roll;
		float y = MaxAngle.Y * FMath::Sin(Frequency.Y * RunnningTime) + StartRotation.Pitch;
		float z = MaxAngle.Z * FMath::Sin(Frequency.Z * RunnningTime) + StartRotation.Yaw;
		Parent->SetActorRotation(FRotator(y, z, x));
		RunnningTime += DeltaTime;
	}
}

