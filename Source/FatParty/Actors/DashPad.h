#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DashPad.generated.h"

class AKnightCharacter;
class UStaticMeshComponent;
class UBoxComponent;
class UArrowComponent;

UCLASS()
class FATPARTY_API ADashPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADashPad();

	UPrimitiveComponent* ObjectToThrow = nullptr;
protected:


	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* OverlapComp;

	UFUNCTION()
		void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	/* Total impulse added to the character on overlap
	Marked 'EditInstanceOnly' to allow in-level editing of this property per instance. */
	UPROPERTY(EditInstanceOnly, Category = "DashPad")
		float LaunchStrength;
	/* Angle added on top of actor rotation to launch the character.
	Marked 'EditInstanceOnly' to allow in-level editing of this property per instance. */
	UPROPERTY(EditInstanceOnly, Category = "DashPad")
		float LaunchPitchAngle;
	/* Effect to play when activating launch pad */
	UPROPERTY(EditDefaultsOnly, Category = "DashPad", BlueprintReadOnly)
		UParticleSystem* ActivateLaunchPadEffect;

private:
	FTimerHandle HasJumped;
	float TimeInAir = 0.3f;

	void Jumped();

	AKnightCharacter* Player = nullptr;

};
