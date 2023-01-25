#include "KnightCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "FatParty/Actors/Projectile.h"
#include "FatParty/Components/Thrower.h"

AKnightCharacter::AKnightCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
}

void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();
	KnightPlayerController = Cast<APlayerController>(GetController());
}

void AKnightCharacter::Fire()
{
	AKnightCharacter* KnightCharacter = Cast<AKnightCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (KnightCharacter->ActorGrabbed == nullptr)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());

		//  Se utiliza para que el projectile sea su propio Actor al ser disparado y no forme parte del tanque / torreta.
		Projectile->SetOwner(this);
	}

	if (KnightCharacter)
	{
		PlayerThrower = Cast<UThrower>(KnightCharacter->PlayerThrower);
		if (KnightCharacter->ActorGrabbed != nullptr && PlayerThrower)
		{
			PlayerThrower->Throw();
		}
	}
}

void AKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFatPartyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFatPartyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFatPartyCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AFatPartyCharacter::Fire);

}

void AKnightCharacter::HandleDestruction()
{
	Super::HandleDestruction(); 
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void AKnightCharacter::SetGrabber(UGrabber* Grabber)
{
	PlayerGrabber = Grabber;
}

void AKnightCharacter::SetThrower(UThrower* Thrower)
{
	PlayerThrower = Thrower;
}

void AKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(KnightPlayerController)
	{
		FHitResult HitResult;
		KnightPlayerController->GetHitResultUnderCursor(
			ECC_Visibility, 
			false,
			HitResult);

		RotateToCharacter(HitResult.ImpactPoint);
	
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint,25.f,	12,	FColor::Blue,false,	-1.f);
	}
}