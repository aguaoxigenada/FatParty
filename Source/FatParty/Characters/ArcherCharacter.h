#pragma once

#include "CoreMinimal.h"
#include "FatParty/FatPartyCharacter.h"
#include "ArcherCharacter.generated.h"

class UGrabber;
class UThrower;

UCLASS()
class FATPARTY_API AArcherCharacter : public AFatPartyCharacter
{
	GENERATED_BODY()
	
	
public:
	AArcherCharacter();

	UPROPERTY(EditAnywhere)
		UAnimMontage* AttackAnim = nullptr;

	void CharacterAttack();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void HandleDestruction() override;

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
	APlayerController* ArcherPlayerController;

	float MaxDistance = 100.f;
	float SphereRadius = 3.f;
	bool bCanJump;

	FTimerHandle TimeToRotate;
	float Duration = 1.2f;

	FTimerHandle StopTheJump;
	float StopTime = 0.5f;
};
