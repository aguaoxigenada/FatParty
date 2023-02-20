#pragma once

#include "CoreMinimal.h"
#include "FatParty/FatPartyEnemy.h"
#include "LongDistanceEnemies.generated.h"

class AFatPartyCharacter;

UCLASS()
class FATPARTY_API ALongDistanceEnemies : public AFatPartyEnemy
{
	GENERATED_BODY()

public:
	virtual void RotateToCharacter(FVector LookAtTarget);
	AFatPartyCharacter* FatPartyCharacter;
	UPROPERTY(EditAnywhere)
		UAnimationAsset* AttackAnim = nullptr;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TSubclassOf<class AProjectile> ProjectileClass;

	virtual void BeginPlay() override;
	virtual void HandleDestruction() override;
	
	//UFUNCTION(NetMulticast, Reliable)
		virtual void Fire();

	UFUNCTION(Server, Reliable)
		virtual void ServerFire();

	void SpawnProjectile();
	bool InFireRange();



private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		float FireRange = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		float FireRate = 2.f;

	UPROPERTY(EditAnywhere)
		float ShotDelay = 0;

	void CheckFireCondition();
	
	FTimerHandle FireRateTimerHandle;

};
