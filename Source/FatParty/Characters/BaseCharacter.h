#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class FATPARTY_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseCharacter();
	void HandleDestruction();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMesh;

	// Meta sirve para que las variables privadas puedan ser expuestas en el editor.
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components", meta = (AllowPrivateAccess = "true")) 
	// class UCapsuleComponent* CapsuleComp;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Turret Components")
	UStaticMeshComponent* BaseMesh;

	void RotateTurret(FVector LookAtTarget);
    void Fire();
	
private:
	
	// Forward declaration:  Sirve para poder poner un componente sin su include y sin sus dependencias.  Sirve para declararlo y no utilizarlo como include.
	// Como buena practica es mejor utilizar el Forward declaration en los archivos .h y en el .cpp si incluir el archivo necesario.
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret Components", meta = (AllowPrivateAccess = "true"))	
	USceneComponent* ProjectileSpawnPoint;

	// TSubclassOf guarda la subclase elegida en el editor, como el blueprint BP_Projectile 
	// En este caso sirve para tener una referencia del blueprint que usa la clase de AProjectile
	// Permite seleccionar cualquier objeto que utilice AProjectile desde el editor y usarlo como el projectil.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;

};

/*
	No se necesita un header  para declarr un puntero, se puede usar
	el forward declaration.

	-Si se necesita el header para construir un objeto.
	-Si se necesita para acceder a los miembros.
	-Si se necesita para Inheritance

	Pragma Once no permite que se repitan los included.
*/
