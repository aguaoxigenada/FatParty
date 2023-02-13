#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"


UCLASS()
class FATPARTY_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Setup(bool IsHud);
	virtual void Teardown();
	virtual void SetMenuInterface(IMenuInterface* TheMenuInterface);


protected:
	IMenuInterface* MenuInterface;
};
