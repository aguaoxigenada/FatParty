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
	//OverlapComp->OnComponentEndOverlap.AddUniqueDynamic(this, &ADashPad::OnOverlapLaunchPad);

	LaunchStrength = 1500;
	LaunchPitchAngle = 35.0f;

}

void ADashPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;


	//TODO: RESOLVER SI TANKE ES CHARACTER O COMO APLICAR EL IMPULSO

	ABasePawn* OtherCharacter = Cast<ABasePawn>(OtherActor);

	if (OtherCharacter)
	{
		// Launch Player! Both booleans give consistent launch velocity by ignoring the current player velocity
		UPrimitiveComponent* objectToThrow = Cast<UPrimitiveComponent>(OtherCharacter->GetRootComponent());
		FVector PlayerForward = OtherCharacter->GetActorForwardVector();
		FVector ImpulseDirection = FVector(PlayerForward.X, 0, 1);
		objectToThrow->AddImpulse(ImpulseDirection * 50000);
	

		// Spawn FX
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		// Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}

}

void ADashPad::OnOverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABasePawn* OtherCharacter = Cast<ABasePawn>(OtherActor);
	//OtherCharacter->

	if (OtherCharacter)
	{
		UPrimitiveComponent* objectToThrow = Cast<UPrimitiveComponent>(OtherCharacter->GetRootComponent());
		objectToThrow->SetSimulatePhysics(false);
		
	}
}



