#pragma once

#include "CoreMinimal.h"
#include "FatParty/BasicEnemies/FatPartyEnemy.h"
#include "AI_CharacterBase.generated.h"

class UBehaviorTree;
class USphereComponent;


UCLASS()
class FATPARTY_API AAI_CharacterBase : public AFatPartyEnemy
{
	GENERATED_BODY()

public:

	UBehaviorTree* GetBehaviorTree() const;
	void ReciveDamage(float damage);
	AAI_CharacterBase();

protected:

	void HandleDestruction() override;

	UPROPERTY(EditAnywhere, Category = AI)
		UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere,Category=AI_STATS)
		float Max_life = 100.0f;
	UPROPERTY(EditAnywhere, Category = AI_STATS)
		float Current_Life = 100.0f;
	UPROPERTY(EditAnywhere, Category = AI_STATS)
		float Attack_Damage = 20.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		UStaticMeshComponent* WeaponMesh;
	/*UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponentAttack;*/

	UFUNCTION(BlueprintCallable)
		void TriggerOnAttack();

	
private:

	void CheckCharacterStats();

	UFUNCTION()
		void OnSphereComponentAttackOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnSphereComponentAttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
