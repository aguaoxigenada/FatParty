#pragma once

#include "CoreMinimal.h"
#include "FatParty/FatPartyCharacter.h"
#include "KnightCharacter.generated.h"

class UGrabber;
class UThrower;

UCLASS()
class FATPARTY_API AKnightCharacter : public AFatPartyCharacter
{
	GENERATED_BODY()
	
	
public:
	AKnightCharacter();

	UPROPERTY(EditAnywhere)
		UAnimMontage* AttackAnim = nullptr;

	UPROPERTY(EditAnywhere)
		float Damage = 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Weapon")
		UStaticMeshComponent* Weapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* Body = nullptr;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCapsuleComponent* HitPoint = nullptr;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Holding = false;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	


	void CharacterAttack();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void HandleDestruction() override;

	APlayerController* GetTankPlayerController() const { return KnightPlayerController; }
	
	bool bHadJumped = false;
	float Distance = 0.f;
		
protected:
	virtual void BeginPlay() override;
	
private:
	APlayerController* KnightPlayerController;

	float MaxDistance = 100.f;
	float SphereRadius = 3.f;
	bool bCanJump;

	FTimerHandle TimeToRotate;
	float Duration = 1.2f;

	FTimerHandle StopTheJump;
	float StopTime = 0.5f;
};
