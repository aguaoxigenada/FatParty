#pragma once

#include "CoreMinimal.h"
#include "FatParty/BasicEnemies/LongDistanceEnemies/LongDistanceEnemies.h"
#include "Tower.generated.h"

UCLASS()
class FATPARTY_API ATower : public ALongDistanceEnemies
{
	GENERATED_BODY()

public:
	ATower();
	virtual void Tick(float DeltaTime) override;
	virtual void RotateToCharacter(FVector LookAtTarget) override;
	virtual void HandleDestruction() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components")
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMesh;

};
