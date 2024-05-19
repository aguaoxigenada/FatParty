#include "HudWidget.h"
#include "FatParty/Characters/FatPartyCharacter.h"
#include "FatParty/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

void UHudWidget::NativeOnInitialized()
{
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

		HealthComp->OnHealthChange.AddDynamic(this, &UHudWidget::PlayerDamaged);
	}
}

void UHudWidget::GameTimer()
{
	FTimerDelegate GameOverTimerDelegate = FTimerDelegate::CreateUObject(
		this,												   // Clase a utilizar
		&UHudWidget::PlayerLost                                // Funcion de la clase
														       // Parametro pasado a la clase (si se quiere)
	);


	// Start Repeating function
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHudWidget::RepeatingVisualTimer, 1.0f, true, 0.f);
	
	// Start GameOverTimer
	GetWorld()->GetTimerManager().SetTimer(
		GameOverTimerHandle,
		GameOverTimerDelegate,                           
		TimerCount,
		false
	);

}

void UHudWidget::UpdateTime(int newTime)
{
//	HudTimer->SetText(FText::FromString(FString::FromInt(newTime)));
}

void UHudWidget::RepeatingVisualTimer()
{
	if (HealthComp->GetHealth() <= 0/*|| PlayerWon*/ )
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	
	TimerCount--;

}

void UHudWidget::PlayerLost()
{
	AController* MyOwnerInstigator = Character->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();
	UGameplayStatics::ApplyDamage(Character, 1000, MyOwnerInstigator, Character, DamageTypeClass);
}

void UHudWidget::PlayerDamaged()
{
	GetHealth();
}

