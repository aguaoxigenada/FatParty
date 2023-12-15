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

void AFatPartyEnemy::MulticastDeathParticles_Implementation()
{
	ServerDeathParticles();
	
}

void AFatPartyEnemy::ServerDeathParticles_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
}


void AFatPartyEnemy::Multicast_HandleDestruction_Implementation()
{
	if (DeathParticles)
	{
		if(HasAuthority())
		{
			MulticastDeathParticles();
		}
		else
		{
			ServerDeathParticles();
		}
		
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


void AFatPartyEnemy::Multicast_PlayAnimation_Implementation(UAnimSequence* AnimToPlay)
{
	GetMesh()->PlayAnimation(AnimToPlay,false);

}

void AFatPartyEnemy::HandleDestruction()
{
	Server_HandleDestruction();
	
}



