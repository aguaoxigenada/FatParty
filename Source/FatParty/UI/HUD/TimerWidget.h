#pragma once

#include "CoreMinimal.h"
#include "FatParty/UI/MenuSystem/MenuWidget.h"
#include "TimerWidget.generated.h"

class UTextBlock;
class UHealthComponent;
class AFatPartyCharacter;
class UProgressBar;

UCLASS()
class FATPARTY_API UTimerWidget : public UMenuWidget
{
	GENERATED_BODY()

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

	FTimerHandle TimerHandle;
	FTimerHandle GameOverTimerHandle;
};
