#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

class UGrabber;
class UThrower;

UCLASS()
class FATPARTY_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
	
public:
	// Sets default values for this pawn's properties
	ATank();
	
		// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

	void SetTankGrabber(UGrabber* grabber);
	void SetTankThrower(UThrower* thrower);
	void Turn(float Value);

	AActor* ActorGrabbed = nullptr;
	UGrabber* TankGrabber = nullptr;
	UThrower* TankThrower = nullptr;
	
	bool bAlive = true;
	bool bHadJumped = false;
	float Distance = 0.f;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, Category = "Components") 
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 100.f;

	void Move(float Value);
	void MoveSides(float Value);
	void CanRotateCam();
	void Jump();
	void StopJump();

	APlayerController* TankPlayerController;

	bool CanJump(FHitResult& OutHitResult) const;
	float MaxDistance = 100.f;
	float SphereRadius = 3.f;
	bool bCanJump;

	FTimerHandle TimeToRotate;
	float Duration = 1.2f;

	FTimerHandle StopTheJump;
	float StopTime = 0.5f;

};
