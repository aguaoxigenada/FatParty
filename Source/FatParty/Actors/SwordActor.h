#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordActor.generated.h"

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroyedSword);

UCLASS()
class FATPARTY_API ASwordActor : public AActor
{
	GENERATED_BODY()

public:
	ASwordActor();

	UPROPERTY()
		FOnDestroyedSword OnDestroyedSword;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* WeaponMesh;  

	double ActualZ;
	double ZVectorLimit = -2000;
};
