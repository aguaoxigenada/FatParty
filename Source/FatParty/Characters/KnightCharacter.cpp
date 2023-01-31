#include "KnightCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FatParty/Components/Thrower.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"


AKnightCharacter::AKnightCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
	Body = GetMesh();

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blade"));
	Weapon->SetupAttachment(Body);

	


	HitPoint = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hit Point"));
	HitPoint->SetupAttachment(Weapon);

}

void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();
	KnightPlayerController = Cast<APlayerController>(GetController());
	Weapon->AttachToComponent(Body,FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHandSocket"));
}


void AKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFatPartyCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFatPartyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFatPartyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFatPartyCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AKnightCharacter::CharacterAttack);

}

void AKnightCharacter::HandleDestruction()
{
	Super::HandleDestruction(); 
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
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
	}
	
}

void AKnightCharacter::CharacterAttack() 
{
	AKnightCharacter* KnightCharacter = Cast<AKnightCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (KnightCharacter)
	{
		PlayerThrower = Cast<UThrower>(KnightCharacter->PlayerThrower);
		if (KnightCharacter->ActorGrabbed != nullptr && PlayerThrower)
		{
			PlayerThrower->Throw();
		}

		else
			PlayAnimMontage(AttackAnim);
	}
}