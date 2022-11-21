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
		ObjectToThrow = Cast<UPrimitiveComponent>(Player->GetRootComponent());
		FVector PlayerForward = Player->GetActorForwardVector();
		FVector ImpulseDirection = FVector(PlayerForward.X, 0, 1);
		Player->bHadJumped = true;
		ObjectToThrow->SetSimulatePhysics(true);
		ObjectToThrow->AddImpulse(ImpulseDirection * 50000);
		GetWorldTimerManager().SetTimer(HasJumped, this, &ADashPad::Jumped, TimeInAir, false);

		// Spawn FX
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		//OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

		// Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}

}

void ADashPad::Jumped()
{
	Player->bHadJumped = false;
}




