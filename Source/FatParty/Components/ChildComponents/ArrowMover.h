#pragma once

#include "CoreMinimal.h"
#include "FatParty/Components/Mover.h"
#include "ArrowMover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UArrowMover : public UMover
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	float Speed;
	bool bStartAction;
	FVector TargetLocation;
	FVector CurrentLocation;
	FVector NewLocation;
	FVector StartLocation;	

	bool ShouldArrowReturn() const;
	float GetDistanceMoved() const;
};
