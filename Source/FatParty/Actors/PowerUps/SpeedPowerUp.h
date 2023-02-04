#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "SpeedPowerUp.generated.h"

UCLASS()
class FATPARTY_API ASpeedPowerUp : public ABasePowerUp
{
	GENERATED_BODY()

public:
	ASpeedPowerUp();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};


