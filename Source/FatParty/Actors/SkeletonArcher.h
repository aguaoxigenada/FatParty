#pragma once

#include "CoreMinimal.h"
#include "FatParty/LongDistanceEnemies/LongDistanceEnemies.h"
#include "SkeletonArcher.generated.h"

UCLASS()
class FATPARTY_API ASkeletonArcher : public ALongDistanceEnemies
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		UAnimationAsset* AttackAnim = nullptr;

	ASkeletonArcher();
	virtual void Tick(float DeltaTime) override;
	virtual void RotateToCharacter(FVector LookAtTarget) override;
	virtual void HandleDestruction() override;

	/*UFUNCTION(NetMulticast, Reliable)
		virtual void Fire() override;*/

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skeleton Components")
		UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skeleton Components", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* HelmetMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skeleton Components", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* QuiverMesh;

};
