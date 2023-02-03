#include "FatPartyCharacter.h"
#include "Actors/Projectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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

void AFatPartyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AFatPartyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MovementComponent->IsFalling())
	{
		float ActualTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);

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
	JumpTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);
	Jump();
	PlayAnimMontage(JumpAnim);
}

void AFatPartyCharacter::SetGrabber(UGrabber* Grabber)
{
	PlayerGrabber = Grabber;
}

void AFatPartyCharacter::SetThrower(UThrower* Thrower)
{
	PlayerThrower = Thrower;
}

void AFatPartyCharacter::HandleDestruction()
{
	// 1. Chequeo de Seguridad que existe el puntero en todos los ifs.
	// 2. Se crean particulas al morir, sonido y movimiento de camara.

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

void AFatPartyCharacter::RotateToCharacter(FVector LookAtTarget)   // hay que cambiar esto para que rote el body
{
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
    
	FVector ToTarget = LookAtTarget - SkeletalMesh->GetComponentLocation();

	// La posicion del FRotator hacia donde debe de mirar la Torreta
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	// Rotacion de la torreta desde su posicion actual hacia LookAtRotation.
	SkeletalMesh->SetWorldRotation(
		FMath::RInterpTo(
			SkeletalMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			25.f));
}

void AFatPartyCharacter::Turn(float Value)
{
	//FRotator DeltaRotation = FRotator::ZeroRotator;
	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);

	// Yaw = Value * DeltaTime * TurnRate
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

	AddActorLocalRotation(DeltaRotation, true);

}

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


