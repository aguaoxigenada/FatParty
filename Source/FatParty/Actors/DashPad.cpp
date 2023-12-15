#include "DashPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FatParty/Characters/FatPartyCharacter.h"
#include "Kismet/GameplayStatics.h"

ADashPad::ADashPad()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	//MeshComp->SetupAttachment(OverlapComp);

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComp->SetBoxExtent(FVector(75, 75, 50));
	OverlapComp->SetupAttachment(MeshComp);

	OverlapComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADashPad::OverlapLaunchPad);
	
	LaunchStrength = 1500;
//	LaunchPitchAngle = 35.0f;
}


void ADashPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	/*
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;
	*/
	Player = Cast<AFatPartyCharacter>(OtherActor);

	if (Player)
	{
		Player->Multicast_JumpAnimation();
		Player->LaunchCharacter(FVector(0,0,LaunchStrength), false, true);

		//Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());

		if (OnOverlapSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OnOverlapSound, Player->GetActorLocation());
		}
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		// Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
}

