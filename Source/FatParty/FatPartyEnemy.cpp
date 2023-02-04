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

void AFatPartyEnemy::BeginPlay()
{
    Super::BeginPlay();

}

void AFatPartyEnemy::HandleDestruction()
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



