#pragma once

#include "CoreMinimal.h"
#include "FatParty/FatPartyCharacter.h"
#include "Tower.generated.h"

UCLASS()
class FATPARTY_API ATower : public AFatPartyCharacter
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();

	virtual void Fire() override;

protected:
	virtual void BeginPlay() override;

private:

	class AKnightCharacter* KnightCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRange = 300.f;

	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRate = 2.f;
	
	void CheckFireCondition();

	bool InFireRange();
};
