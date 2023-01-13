#include "ArrowMover.h"
#include "Math/UnrealMathUtility.h"

void UArrowMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldMove)
	{
		bStartAction = true;
	}

	CurrentLocation = GetOwner()->GetActorLocation();
	Speed = MoveOffset.Length() / MoveTime;

	if(bStartAction)
	{
		if(ShouldArrowReturn())
		{
			StartLocation = StartLocation + MoveOffset;
			GetOwner()->SetActorLocation(StartLocation);
			MoveOffset = MoveOffset * -1;
		}

		else
		{
			TargetLocation = OriginalLocation + MoveOffset;
			NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			GetOwner()->SetActorLocation(NewLocation);
		}
	}
	
}

void UArrowMover::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetOwner()->GetActorLocation();
}


bool UArrowMover::ShouldArrowReturn() const
{
	float Distance = GetDistanceMoved();
	float Offset = MoveOffset.Length();
	UE_LOG(LogTemp, Warning, TEXT("Distance is: %f"), Distance);
	UE_LOG(LogTemp, Warning, TEXT("MoveOffset is: %f"), Offset);

	return GetDistanceMoved() >= MoveOffset.Length();
}

float UArrowMover::GetDistanceMoved() const
{
	return FVector::Dist(StartLocation, CurrentLocation);
}

