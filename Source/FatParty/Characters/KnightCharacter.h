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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Weapon")
		UStaticMeshComponent* Weapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* Body = nullptr;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_CharacterAttack();

	void CharacterAttack();

	void CharacterCanAttack();

	void SetCooldown();
	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Unreliable)
		virtual void HandleDestruction() override;

	APlayerController* GetKnightController() const { return KnightPlayerController; }
	
	bool bHadJumped = false;
	float Distance = 0.f;
	bool CanAttack = true;

	FTimerHandle AttackCooldown;
		
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
