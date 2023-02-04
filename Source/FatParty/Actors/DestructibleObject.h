#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleObject.generated.h"

class ASpeedPowerUp;
class ALifePowerUp;
class ABasePowerUp;
class UHealthComponent;
class USoundBase;

UCLASS()
class FATPARTY_API ADestructibleObject : public AActor
{
	GENERATED_BODY()

public:
	ADestructibleObject();
	void HandleDestruction();
	void SetObjectToDrop(ABasePowerUp* PickedItem);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		float DamageToEnemy = 10.f;

	UPROPERTY(EditAnywhere)
		float DamageToSelf = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Object")
		UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere, Category = "Object")
		class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Object")
		USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Object")
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Drop Object")
		ABasePowerUp* ObjectToDrop;

	UPROPERTY()
		UClass* GenericBuildingClass;

	UFUNCTION()
		void HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );


	float Velocity = 0.f;
	UHealthComponent* HealthComp;
};


