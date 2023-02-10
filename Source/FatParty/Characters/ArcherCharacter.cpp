#include "ArcherCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "FatParty/Actors/Projectile.h"
#include "FatParty/Components/Thrower.h"
#include "FatParty/Controllers/ThePlayerController.h"
#include "Kismet/KismetMathLibrary.h"


AArcherCharacter::AArcherCharacter()
{
	Body = GetMesh();

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bow"));
	Weapon->SetupAttachment(Body);
}

void AArcherCharacter::BeginPlay()
{
	Super::BeginPlay();
	ArcherPlayerController = Cast<APlayerController>(GetController());

	Weapon->AttachToComponent(Body,FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHandSocket"));
}

void AArcherCharacter::Fire()
{

	AArcherCharacter* ArcherCharacter = Cast<AArcherCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (ArcherCharacter->ActorGrabbed == nullptr)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());

		Projectile->SetOwner(this);
	} 

	if (ArcherCharacter)
	{
		PlayerThrower = Cast<UThrower>(ArcherCharacter->PlayerThrower);
		if (ArcherCharacter->ActorGrabbed != nullptr && PlayerThrower)
		{
			PlayerThrower->Throw();
			PlayAnimMontage(AttackAnim);
		}
	}
}

void AArcherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFatPartyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFatPartyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFatPartyCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AArcherCharacter::Fire);

}

void AArcherCharacter::HandleDestruction()
{
	Super::HandleDestruction(); 
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}


void AArcherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	/*if(ArcherPlayerController)
	{
		FHitResult HitResult;
		ArcherPlayerController->GetHitResultUnderCursor(
			ECC_Visibility, 
			false,
			HitResult);

		// Mouse position
		AArcherCharacter* ArcherCharacter = Cast<AArcherCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ArcherCharacter->GetActorLocation(), HitResult.ImpactPoint);
		//break rotation sacar el vector de yaw y mandarlo.


		FRotator RotateTo = FRotator(0, LookAtRotation.Pitch, 0);
		FVector RotateToVector =  FVector(0, RotateTo.Pitch, 0);

		RotateToCharacter(RotateToVector);
	
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint,25.f,	12,	FColor::Blue,false,	-1.f);

		//CharacterMouseRotator();
	}	*/
}
