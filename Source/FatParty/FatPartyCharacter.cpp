#include "FatPartyCharacter.h"
#include "Actors/Projectile.h"
#include "Camera/CameraComponent.h"
#include "Characters/KnightCharacter.h"
#include "Components/Thrower.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

AFatPartyCharacter::AFatPartyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);


	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Pont"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}

void AFatPartyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}


void AFatPartyCharacter::HandleDestruction()
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

void AFatPartyCharacter::Fire()
{
	

}

void AFatPartyCharacter::RotateTurret(FVector LookAtTarget)
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

void AFatPartyCharacter::Turn(float Value)
{
	//FRotator DeltaRotation = FRotator::ZeroRotator;
	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);

	// Yaw = Value * DeltaTime * TurnRate
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

	AddActorLocalRotation(DeltaRotation, true);

}

void AFatPartyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value * Speed);
	}
}

void AFatPartyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value * Speed);
	}
}

