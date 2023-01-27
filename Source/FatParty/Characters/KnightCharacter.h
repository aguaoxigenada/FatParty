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

	void CharacterAttack();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void HandleDestruction() override;

	APlayerController* GetTankPlayerController() const { return KnightPlayerController; }

	void SetGrabber(UGrabber* Grabber);
	void SetThrower(UThrower* Thrower);
	void Turn(float Value);

	AActor* ActorGrabbed = nullptr;
	UGrabber* PlayerGrabber = nullptr;
	UThrower* PlayerThrower = nullptr;

	bool bHadJumped = false;
	float Distance = 0.f;
		
protected:
	virtual void BeginPlay() override;
	virtual void Fire() override;
	
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
