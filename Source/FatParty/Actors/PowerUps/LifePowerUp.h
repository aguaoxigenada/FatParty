#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "LifePowerUp.generated.h"

UCLASS()
class FATPARTY_API ALifePowerUp : public ABasePowerUp
{
	GENERATED_BODY()

public:
	ALifePowerUp();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
		float LifeToAdd = 10.f;

	UFUNCTION()
		void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};


