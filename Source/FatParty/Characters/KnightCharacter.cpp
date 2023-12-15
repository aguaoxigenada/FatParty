#include "KnightCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "FatParty/Components/Thrower.h"
#include "TimerManager.h"


AKnightCharacter::AKnightCharacter()
{
	Body = GetMesh();

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blade"));
	Weapon->SetupAttachment(Body);
	
}

void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();
	KnightPlayerController = Cast<APlayerController>(GetController());
	Weapon->AttachToComponent(Body,FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHandSocket"));

	Weapon->OnComponentHit.AddDynamic(this, &AKnightCharacter::OnHit);
}


void AKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFatPartyCharacter::Multicast_JumpAnimation);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFatPartyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFatPartyCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AKnightCharacter::CharacterCanAttack);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AKnightCharacter::StartCooldown);

}

void AKnightCharacter::HandleDestruction_Implementation()
{
	Super::HandleDestruction(); 
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

/*void AKnightCharacter::Tick(float DeltaTime)
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
	
}*/

void AKnightCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
	}
}


void AKnightCharacter::Multicast_CharacterAttack_Implementation()
{
	CharacterAttack();
}

void AKnightCharacter::CharacterAttack()
{
	UWorld* World = GetWorld();
	AKnightCharacter* KnightCharacter = Cast<AKnightCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (KnightCharacter)
	{
		PlayerThrower = Cast<UThrower>(KnightCharacter->PlayerThrower);
		if (KnightCharacter->ActorGrabbed != nullptr && PlayerThrower)
		{
			PlayerThrower->Server_Throw();
		}

		else
			Server_PlayAnimation(AttackAnim);
			UGameplayStatics::PlaySound2D(World, AttackSound, 1.0f, 1.0f);
	}
}

void AKnightCharacter::CharacterCanAttack()
{
	if (CanAttack) {
		CanAttack = false;
		Multicast_CharacterAttack();
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(AttackCooldown, this, &AKnightCharacter::SetCooldown, 0.5f, false);
	}
}
	

void AKnightCharacter::SetCooldown()
{
	CanAttack = true;
}

void AKnightCharacter::MoveForward(float Value)
{
	
	if (CanAttack) {
		Super::MoveForward(Value);
		
	}
}

void AKnightCharacter::MoveRight(float Value)
{
	if (CanAttack) {
		Super::MoveRight(Value);
	}
	
}

void AKnightCharacter::StartCooldown()
{
	GetWorld()->GetTimerManager().SetTimer(AttackCooldown, this, &AKnightCharacter::SetCooldown, 0.5f, false);
}
