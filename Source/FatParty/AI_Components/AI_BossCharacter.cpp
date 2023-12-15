#include "AI_BossCharacter.h"
#include "FatParty/Components/HealthComponent.h"

void AAI_BossCharacter::BeginPlay()
{
	Super::BeginPlay();
	BossStateValue = 0;
	HealthCompt = Cast<UHealthComponent>( this->FindComponentByClass(UHealthComponent::StaticClass()));
	
}

UHealthComponent* AAI_BossCharacter::GetHealthComponent()
{
	return HealthCompt;
}




