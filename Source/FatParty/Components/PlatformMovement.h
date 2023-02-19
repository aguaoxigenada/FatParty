#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlatformMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UPlatformMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlatformMovement();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetShouldMove(bool ShouldMove);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Movement Platform")
		FVector PlatformVelocity = FVector(100, 0, 0);

	UPROPERTY(EditAnywhere, Category = "Movement Platform")
		float MoveDistance = 100;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float RotationDistance = 50;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		FVector RotationVelocity = FVector(50, 0, 0);

	UPROPERTY(EditInstanceOnly, Category = "Movement Platform")
		bool bCanMove = false;

	FVector StartLocation;
	FRotator StartRotation;
	FRotator TargetRotation;

	bool bStartingOver = true;


	void MovePlatform(float DeltaTime);
	void RotatePlatform(float DeltaTime);

	bool ShouldPlatformReturn() const;
	bool ShouldRotationReturn() const;
	float GetDistanceMoved() const;
	float GetDistanceRotated() const;
};
