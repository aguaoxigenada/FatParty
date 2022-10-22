// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Thrower.h"
#include "Grabber.h"

ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Aca se crea la base del Tanque y de la Torreta, ambos comparten todo esto
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Pont"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
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

void ABasePawn::HandleDestruction()
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

void ABasePawn::Fire()
{
	ATank* Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
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

// Funcion para disparar.

// auto setea automaticamente el tipo de objeto, es util usarlo cuando no se esta seguro que retorna un puntero.
// No es buena practica usarlo, pero si es util cuando no se esta seguro lo que retorna, para luego modificarlo.

/* En este caso retorna  AProjectile*/ 
	//si el player no esta agarrando algo 






}
