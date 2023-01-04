#include "Tower.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "FatParty/Characters/KnightCharacter.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (InFireRange())
    {
        RotateTurret(KnightCharacter->GetActorLocation());
    }
}

void ATower::BeginPlay()
{
    Super::BeginPlay();

    KnightCharacter = Cast<AKnightCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{
    if(KnightCharacter == nullptr)
    {
        return;
    }

    if (InFireRange() && KnightCharacter->bAlive)
    {
        Fire();
    }
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction(); // llama a la base!
    Destroy();
}

bool ATower::InFireRange()
{
    if (KnightCharacter)
    {
        float Distance = FVector::Dist(GetActorLocation(), KnightCharacter->GetActorLocation());
        if (Distance <= FireRange)
        {
            return true;
        }
    }

    return false;
}

void ATower::Fire()
{
    AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
        ProjectileClass,
        ProjectileSpawnPoint->GetComponentLocation(),
        ProjectileSpawnPoint->GetComponentRotation());

	//  Se utiliza para que el projectile sea su propio Actor al ser disparado y no forme parte del tanque / torreta.
	Projectile->SetOwner(this);
}