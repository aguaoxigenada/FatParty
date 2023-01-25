#include "LongDistanceEnemies.h"
#include "FatParty/FatPartyCharacter.h"
#include "FatParty/Actors/Projectile.h"
#include "Kismet/GameplayStatics.h"

void ALongDistanceEnemies::BeginPlay()
{
    Super::BeginPlay();

    FatPartyCharacter = Cast<AFatPartyCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ALongDistanceEnemies::CheckFireCondition, FireRate, true);
}

void ALongDistanceEnemies::CheckFireCondition()
{
    if(FatPartyCharacter == nullptr)
    {
        return;
    }

    if (InFireRange() && FatPartyCharacter->bAlive)
    {
       Fire();
    }
}

void ALongDistanceEnemies::RotateToCharacter(FVector LookAtTarget)  
{
   
	
}

void ALongDistanceEnemies::HandleDestruction()
{
    Super::HandleDestruction(); 
    Destroy();
}

bool ALongDistanceEnemies::InFireRange()
{
    if (FatPartyCharacter)
    {
        float Distance = FVector::Dist(GetActorLocation(), FatPartyCharacter->GetActorLocation());
        if (Distance <= FireRange)
        {
            return true;
        }
    }

    return false;
}

void ALongDistanceEnemies::Fire()
{
	FTimerHandle TimerHandleToOpenInGameMenu;
    GetWorld()->GetTimerManager().SetTimer(TimerHandleToOpenInGameMenu, this, &ALongDistanceEnemies::SpawnProjectile, ShotDelay, false );
}

void ALongDistanceEnemies::SpawnProjectile()
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
        ProjectileClass,
        ProjectileSpawnPoint->GetComponentLocation(),
        ProjectileSpawnPoint->GetComponentRotation());

	Projectile->SetOwner(this);
}

