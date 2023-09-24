#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "NetworkErrorWidget.generated.h"

class UButton;

UCLASS()
class FATPARTY_API UNetworkErrorWidget : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();

private:
	UFUNCTION()
		void GoToMainMenu();
};
