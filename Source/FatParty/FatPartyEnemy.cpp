#include "FatPartyEnemy.h"
#include "Kismet/GameplayStatics.h"

AFatPartyEnemy::AFatPartyEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFatPartyEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AFatPartyEnemy::Server_HandleDestruction_Implementation()
{
	Multicast_HandleDestruction();
}

void AFatPartyEnemy::BeginPlay()
{
    Super::BeginPlay();

}

void AFatPartyEnemy::Multicast_HandleDestruction_Implementation()
{
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

void AFatPartyEnemy::HandleDestruction()
{
	Server_HandleDestruction();
	
}



