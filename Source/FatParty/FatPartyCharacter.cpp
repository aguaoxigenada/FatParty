#include "FatPartyCharacter.h"
#include "Actors/SwordActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

AFatPartyCharacter::AFatPartyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Pont"));
	ProjectileSpawnPoint->SetupAttachment(WeaponMesh);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;             // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);  // ...at this rotation rate
	GetCharacterMovement()->AirControl = 0.2f;

	MovementComponent = GetCharacterMovement();
}

void AFatPartyCharacter::HitAnimation()
{
	Server_PlayAnimation(HitAnim);
}

void AFatPartyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFatPartyCharacter, JumpTime);
}

void AFatPartyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	TArray<AActor*> PosibleActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASwordActor::StaticClass(), PosibleActors);

	for(AActor *Actors : PosibleActors)
	{
		ASwordActor* ThisActor = Cast<ASwordActor>(Actors);
		if(ThisActor)
		{
            SwordObject = ThisActor;
		}
	}

	if(SwordObject == nullptr) return;
	SwordObject->OnDestroyedSword.AddDynamic(this, &AFatPartyCharacter::KillAllPlayers);

	
}

void AFatPartyCharacter::KillAllPlayers_Implementation()
{
	AController* MyOwnerInstigator = this->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();
	UGameplayStatics::ApplyDamage(this, 1000, MyOwnerInstigator, this, DamageTypeClass);
	
}

void AFatPartyCharacter::StartExtraSpeedTimer()
{
	FTimerHandle TimerHandleToOpenInGameMenu;
	GetWorldTimerManager().SetTimer(TimerHandleToOpenInGameMenu, this, &AFatPartyCharacter::StopExtraSpeed, TimeDelay, false );
}

void AFatPartyCharacter::Multicast_PlayAnimation_Implementation(UAnimMontage* AnimToPlay)
{
	PlayAnimMontage(AnimToPlay);
}

void AFatPartyCharacter::Server_PlayAnimation_Implementation(UAnimMontage* AnimToPlay)
{
	Multicast_PlayAnimation(AnimToPlay);
}

void AFatPartyCharacter::StopExtraSpeed()
{
	MovementComponent->MaxWalkSpeed = 600;
}

void AFatPartyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MovementComponent->IsFalling())
	{
		ActualTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);

		if(ActualTime - JumpTime >= TimeToDie)
		{
			AController* MyOwnerInstigator = this->GetInstigatorController();
			UClass* DamageTypeClass = UDamageType::StaticClass();
			UGameplayStatics::ApplyDamage(this, 1000, MyOwnerInstigator, this, DamageTypeClass);
		}
	}
}

void AFatPartyCharacter::StartJump()
{
	Jump();
	Server_UpdateJumpTime();
	Server_PlayAnimation(JumpAnim);
}

void AFatPartyCharacter::Multicast_JumpAnimation_Implementation()
{
	StartJump();
}

void AFatPartyCharacter::SetGrabber(UGrabber* Grabber)
{
	PlayerGrabber = Grabber;
}

void AFatPartyCharacter::SetThrower(UThrower* Thrower)
{
	PlayerThrower = Thrower;
}


void AFatPartyCharacter::Server_UpdateJumpTime_Implementation()
{
	JumpTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);
}

void AFatPartyCharacter::HandleDestruction()
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

	bAlive = false;
}

void AFatPartyCharacter::Fire()
{
	

}


/*void AFatPartyCharacter::Turn(float Value)
{
	//FRotator DeltaRotation = FRotator::ZeroRotator;
	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);

	// Yaw = Value * DeltaTime * TurnRate
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

	AddActorLocalRotation(DeltaRotation, true);

}
*/

void AFatPartyCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFatPartyCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


