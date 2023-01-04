// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlatformMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UPlatformMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlatformMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Movement Platform")
		FVector PlatformVelocity = FVector(100, 0, 0);

	UPROPERTY(EditAnywhere, Category = "Movement Platform")
		float MoveDistance = 100;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		FRotator RotationVelocity;

	FVector StartLocation;

	void MovePlatform(float DeltaTime);
	void RotatePlatform(float DeltaTime);

	bool ShouldPlatformReturn() const;
	float GetDistanceMoved() const;

		
};
