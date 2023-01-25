#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FatPartyEnemy.generated.h"

UCLASS()
class FATPARTY_API AFatPartyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AFatPartyEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void HandleDestruction();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;
};
