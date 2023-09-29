#include "TimerWidget.h"
#include "Components/TextBlock.h"
#include "FatParty/FatPartyCharacter.h"
#include "Kismet/GameplayStatics.h"

void UTimerWidget::NativeOnInitialized()
{
	Character = Cast<AFatPartyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	HudTimer = (UTextBlock*)(WidgetTree->FindWidget(FName(TEXT("Timer"))));

		GameTimer();
}

void UTimerWidget::GameTimer()
{
	FTimerDelegate GameOverTimerDelegate = FTimerDelegate::CreateUObject(
		this,												   // Clase a utilizar
		&UTimerWidget::PlayerLost                                // Funcion de la clase
														       // Parametro pasado a la clase (si se quiere)
	);


	// Start Repeating function
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTimerWidget::RepeatingVisualTimer, 1.0f, true, 0.f);
	
	// Start GameOverTimer
	GetWorld()->GetTimerManager().SetTimer(
		GameOverTimerHandle,
		GameOverTimerDelegate,                           
		TimerCount,
		false
	);

}

void UTimerWidget::UpdateTime(int newTime)
{
	HudTimer->SetText(FText::FromString(FString::FromInt(newTime)));
}

void UTimerWidget::RepeatingVisualTimer()
{
	//if (HealthComp->GetHealth() <= 0/*|| PlayerWon*/ )
	//{
	//	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	//}
	
//	TimerCount--;

}

void UTimerWidget::PlayerLost()
{
	AController* MyOwnerInstigator = Character->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();
	UGameplayStatics::ApplyDamage(Character, 1000, MyOwnerInstigator, Character, DamageTypeClass);
}

/*
void UTimerWidget::PlayerDamaged()
{
//	GetHealth();
}

*/
