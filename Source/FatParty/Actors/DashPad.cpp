#include "DashPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "FatParty/Characters/KnightCharacter.h"
#include "Kismet/GameplayStatics.h"

ADashPad::ADashPad()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComp->SetBoxExtent(FVector(75, 75, 50));
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	OverlapComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADashPad::OverlapLaunchPad);
	
	LaunchStrength = 1500;
	LaunchPitchAngle = 35.0f;
}


void ADashPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	Player = Cast<AKnightCharacter>(OtherActor);

	if (Player)
	{
		Player->LaunchCharacter(FVector(0,0,500), false, true);

		// Spawn FX
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		// Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
}

