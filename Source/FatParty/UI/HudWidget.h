#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HudWidget.generated.h"

class UProgressBar;

UCLASS()
class FATPARTY_API UHudWidget : public UUserWidget
{
	GENERATED_BODY()
		
protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* LifeBar;
};
