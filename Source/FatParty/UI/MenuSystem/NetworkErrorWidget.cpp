#include "NetworkErrorWidget.h"

bool UNetworkErrorWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;

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
