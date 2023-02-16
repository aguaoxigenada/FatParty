#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "SwordActor.generated.h"

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroyedSword);

UCLASS()
class FATPARTY_API ASwordActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ASwordActor();

	UPROPERTY()
		FOnDestroyedSword OnDestroyedSword;

	virtual void Tick(float DeltaTime) override;

private:
	UStaticMeshComponent* WeaponMesh;
	UBoxComponent* BoxCollision;

	double ActualZ;
	double ZVectorLimit = -2000;
};
