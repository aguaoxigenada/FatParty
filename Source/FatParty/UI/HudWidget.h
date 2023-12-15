#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "HudWidget.generated.h"

class UTextBlock;
class UHealthComponent;
class AFatPartyCharacter;
class UProgressBar;

UCLASS()
class FATPARTY_API UHudWidget : public UMenuWidget
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

	UFUNCTION(BlueprintCallable)
		void PlayerLost();

	UFUNCTION(BlueprintCallable)
		void UpdateTime(int newTime);

	void GameTimer();


	UHealthComponent* HealthComp;

	FTimerHandle TimerHandle;
	FTimerHandle GameOverTimerHandle;
};
