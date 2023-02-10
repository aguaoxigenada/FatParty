#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FatPartyCharacter.generated.h"

class ASpeedPowerUp;
class UThrower;
class UGrabber;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class AFatPartyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFatPartyCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* JumpAnim = nullptr;

	void Turn(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMesh;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	UFUNCTION(Server, Unreliable)
		void Server_PlayAnimation(UAnimMontage* AnimToPlay);

	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_PlayAnimation(UAnimMontage* AnimToPlay);

	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_JumpAnimation();

	UFUNCTION(Server,Unreliable)
		void Server_UpdateJumpTime();

	virtual void HandleDestruction();
	virtual void Tick(float DeltaTime) override;
	virtual void Fire();

	void RotateToCharacter(FVector LookAtTarget);
	void MoveForward(float Val);
	void MoveRight(float Val);
	void SetGrabber(UGrabber* Grabber);
	void SetThrower(UThrower* Thrower);
	void StartJump();
	void StartExtraSpeedTimer();

	AActor* ActorGrabbed = nullptr;
	UGrabber* PlayerGrabber = nullptr;
	UThrower* PlayerThrower = nullptr;
	bool bAlive = true;


protected:
	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components")
		UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Holding = false;

	UPROPERTY(EditAnywhere)
		float Damage = 50.f;

private:
	UPROPERTY(EditAnywhere, Category = "TurnRate")
		float TurnRate = 100.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed = 0.8f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Death By Fall")
		float TimeToDie = 4.f;

	UPROPERTY(Replicated,VisibleAnywhere, Category = "Death By Fall")
		float JumpTime = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Death By Fall")
		float ActualTime;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;

		/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	void StopExtraSpeed();

	UCharacterMovementComponent* MovementComponent;
	ASpeedPowerUp* SpeedComp;

	float TimeDelay = 3.f;
};

