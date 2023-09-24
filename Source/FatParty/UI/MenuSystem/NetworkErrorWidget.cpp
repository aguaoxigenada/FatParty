#include "NetworkErrorWidget.h"
//#include "Components/Button.h"

bool UNetworkErrorWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;

	//if(!ensure(MainMenuButton!=nullptr)) return false;
	//MainMenuButton->OnClicked.AddDynamic(this, &UNetworkErrorWidget::GoToMainMenu);
		
	return true;
}

void UNetworkErrorWidget::GoToMainMenu()
{
	if(MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadGameMenu();
	}
}
