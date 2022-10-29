#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "KnightCharacter.h"
#include "Camera/CameraComponent.h"
#include "FatParty/Actors/Projectile.h"
#include "FatParty/Components/Thrower.h"
#include "GameFramework/SpringArmComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Pont"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}


void ABaseCharacter::RotateTurret(FVector LookAtTarget)
{
	// Distancia entre el Tanque y la Torreta
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	
	// La posicion del FRotator hacia donde debe de mirar la Torreta
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	
	// Rotacion de la torreta desde su posicion actual hacia LookAtRotation.
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			25.f));
}

void ABaseCharacter::HandleDestruction()
{
	// 1. Chequeo de Seguridad que existe el puntero en todos los ifs.
	// 2. Se crean particulas al morir, sonido y movimiento de camara.

	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}

void ABaseCharacter::Fire()
{
	AKnightCharacter* Tank = Cast<AKnightCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	UThrower* playerThrower = nullptr;
	if (Tank->ActorGrabbed == nullptr)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());

		//  Se utiliza para que el projectile sea su propio Actor al ser disparado y no forme parte del tanque / torreta.
		Projectile->SetOwner(this);
	}


	if (Tank)
	{
		playerThrower = Cast<UThrower>(Tank->TankThrower);
		if (Tank->ActorGrabbed != nullptr && playerThrower)
		{
			playerThrower->Throw();
		}
	}

}
