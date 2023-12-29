#include "InGameMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameSession.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;

	if(!ensure(MenuButton!=nullptr)) return false;
	MenuButton->OnClicked.AddDynamic(this, &UInGameMenu::GoToMainMenu);

	if(!ensure(LevelPassed_MenuButton!=nullptr)) return false;
	LevelPassed_MenuButton->OnClicked.AddDynamic(this, &UInGameMenu::GoToMainMenu);

	if(!ensure(NextLevelButton!=nullptr)) return false;
	NextLevelButton->OnClicked.AddDynamic(this, &UInGameMenu::GoToNextLevel);

	if(!ensure(LevelLost_MenuButton!=nullptr)) return false;
	LevelLost_MenuButton->OnClicked.AddDynamic(this, &UInGameMenu::GoToMainMenu);

	if(!ensure(RetryButton!=nullptr)) return false;
	RetryButton->OnClicked.AddDynamic(this, &UInGameMenu::RetryLevel);

	if(!ensure(CancelButton!=nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelMenu);
	
	return true;
}

void UInGameMenu::CancelMenu()
{
	Teardown();	
}

void UInGameMenu::EndLevelScreen(bool bWon)
{
	if(!ensure(InGameMenuSwitcher!=nullptr)) return;
	if(bWon)
		InGameMenuSwitcher->SetActiveWidget(LevelPassed);
	else 
		InGameMenuSwitcher->SetActiveWidget(LevelLost);	
}


void UInGameMenu::GoToMainMenu()
{
	if(MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadGameMenu();
	}
}

void UInGameMenu::GoToNextLevel()
{
	if(MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadNextLevel();
	}
}

void UInGameMenu::RetryLevel()
{
	if(MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Got Pressed"));
		Teardown();
		MenuInterface->PlayerRetry();
	}
}

