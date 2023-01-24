#include "Tower.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "FatParty/Characters/KnightCharacter.h"

ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	 if (InFireRange())
     {
		RotateToCharacter(FatPartyCharacter->GetActorLocation());
     }
}

void ATower::RotateToCharacter(FVector LookAtTarget)
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

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
}
