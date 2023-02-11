#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "HudWidget.generated.h"

class UTextBlock;
class UHealthComponent;
class AFatPartyCharacter;
class UProgressBar;

UCLASS()
class FATPARTY_API UHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void PlayerDamaged();

	UFUNCTION(BlueprintImplementableEvent)
		void GetHealth();

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
		AFatPartyCharacter* Character;

	UPROPERTY()
		UTextBlock* HudTimer;

	UPROPERTY(EditAnywhere)
		float TimerCount = 240;

	void RepeatingVisualTimer();
	void PlayerLost();
	void GameTimer();

	UHealthComponent* HealthComp;

	FTimerHandle TimerHandle;
	FTimerHandle GameOverTimerHandle;
};
