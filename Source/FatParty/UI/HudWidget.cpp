#include "HudWidget.h"
#include "FatParty/FatPartyCharacter.h"
#include "FatParty/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

void UHudWidget::NativeOnInitialized()
{
	Super::OnInitialized();

	Character = Cast<AFatPartyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if(AFatPartyCharacter* FatCharacter = Character ? Cast<AFatPartyCharacter>(Character) : nullptr)
	{
		TArray<UActorComponent*> Components;

		FatCharacter->GetComponents(Components);

	    for(UActorComponent *Comp : Components)
		{
			UHealthComponent* ThisComp = Cast<UHealthComponent>(Comp);
			if( ThisComp )
			{
	            HealthComp = ThisComp;
			}
		}

		HealthComp->OnPlayerDamaged.AddDynamic(this, &UHudWidget::PlayerDamaged);

	}

}


void UHudWidget::PlayerDamaged()
{
	GetHealth();
}
