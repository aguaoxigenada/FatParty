#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HudWidget.generated.h"

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
	//UPROPERTY(meta = (BindWidget))
	//	UProgressBar* LifeBar;

	UPROPERTY()
		AFatPartyCharacter* Character;

	UHealthComponent* HealthComp;

	//float MaxLife = 100;
	//float CurrentLife = MaxLife;
};
