#include "SkeletonArcher.h"
#include "FatParty/FatPartyCharacter.h"
#include "Kismet/GameplayStatics.h"

ASkeletonArcher::ASkeletonArcher()
{
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Bip001-L-Hand"));

	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet Mesh"));
	HelmetMesh->SetupAttachment(GetMesh());

	QuiverMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Quiver Mesh"));
	QuiverMesh->SetupAttachment(GetMesh());

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

}

void ASkeletonArcher::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	 if (InFireRange())
     {
		RotateToCharacter(FatPartyCharacter->GetActorLocation());
     }
}

void ASkeletonArcher::RotateToCharacter(FVector LookAtTarget)
{
    // Distancia entre el Tanque y la Torreta
	FVector ToTarget = LookAtTarget - RootComponent->GetComponentLocation();

	// La posicion del FRotator hacia donde debe de mirar la Torreta
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	// Rotacion de la torreta desde su posicion actual hacia LookAtRotation.
	RootComponent->SetWorldRotation(
		FMath::RInterpTo(
			RootComponent->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			25.f));
}

void ASkeletonArcher::HandleDestruction()
{
	Super::HandleDestruction();
}

void ASkeletonArcher::Fire()
{
	Super::Fire();
	GetMesh()->PlayAnimation(AttackAnim, false);
}

	
