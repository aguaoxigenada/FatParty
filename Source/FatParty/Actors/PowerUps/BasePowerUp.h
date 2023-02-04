#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePowerUp.generated.h"

class UHealthComponent;
class USoundBase;
class USphereComponent;

UCLASS()
class FATPARTY_API ABasePowerUp : public AActor
{
	GENERATED_BODY()

public:
	ABasePowerUp();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Object")
		USphereComponent* SphereCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Object")
		UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere, Category = "Object")
		USoundBase* PickedSound;
};


