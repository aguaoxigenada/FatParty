// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_CharacterBase.h"

#include "Components/SphereComponent.h"
#include "FatParty/FatPartyCharacter.h"
#include "Kismet/GameplayStatics.h"


UBehaviorTree* AAI_CharacterBase::GetBehaviorTree() const
{
	return BehaviorTree;
}

void AAI_CharacterBase::ReciveDamage(float damage)
{
	Current_Life -= damage;
	
}

AAI_CharacterBase::AAI_CharacterBase()
{

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	WeaponMesh->SetupAttachment(GetMesh(),"Weapon_Root_RSocket");

	/*SphereComponentAttack = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponentAttack"));
	SphereComponentAttack->SetupAttachment(WeaponMesh);

	SphereComponentAttack->SetSphereRadius(100.f);

	SphereComponentAttack->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAI_CharacterBase::OnSphereComponentAttackOverlapped);
	SphereComponentAttack->OnComponentEndOverlap.AddUniqueDynamic(this, &AAI_CharacterBase::OnSphereComponentAttackEndOverlap);
*/
}

void AAI_CharacterBase::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}


void AAI_CharacterBase::CheckCharacterStats()
{
	if(Current_Life <= 0)
	{
		//DIE
		Destroy();
	}
}

void AAI_CharacterBase::OnSphereComponentAttackOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AController* MyOwnerInstigator = GetOwner()->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor->IsA<AFatPartyCharacter>())
	{
		UGameplayStatics::ApplyDamage(OtherActor, Attack_Damage, MyOwnerInstigator, this, DamageTypeClass);

	}
}

void AAI_CharacterBase::OnSphereComponentAttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FString s;
}

void AAI_CharacterBase::TriggerOnAttack()
{
	FString s;
}

