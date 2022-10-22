// Fill out your copyright notice in the Description page of Project Settings.


#include "DashPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"

// Sets default values
ADashPad::ADashPad()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComp->SetBoxExtent(FVector(75, 75, 50));
	//RootComponent = OverlapComp;

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


	//TODO: RESOLVER SI TANKE ES CHARACTER O COMO APLICAR EL IMPULSO

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);

	if (OtherCharacter)
	{
		// Launch Player! Both booleans give consistent launch velocity by ignoring the current player velocity
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		// Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		// Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}

}

