#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FatPartyCharacter.generated.h"

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
	void Turn(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMesh;

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	virtual void HandleDestruction();
	virtual void Fire();
	void MoveForward(float Val);
	void MoveRight(float Val);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components")
		UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;

	// TSubclassOf guarda la subclase elegida en el editor, como el blueprint BP_Projectile 
	// En este caso sirve para tener una referencia del blueprint que usa la clase de AProjectile
	// Permite seleccionar cualquier objeto que utilice AProjectile desde el editor y usarlo como el projectil.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TSubclassOf<class AProjectile> ProjectileClass;
	
	void RotateTurret(FVector LookAtTarget);
	

private:
	UPROPERTY(EditAnywhere, Category = "TurnRate")
		float TurnRate = 100.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed = 0.8f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;
};

