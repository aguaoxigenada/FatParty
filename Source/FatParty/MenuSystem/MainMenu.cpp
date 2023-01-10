#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;

	if(!ensure(HostButton!=nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if(!ensure(JoinButton!=nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if(!ensure(CancelButton!=nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if(!ensure(JoinIPButton!=nullptr)) return false;
	JoinIPButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if(!ensure(QuitButton!=nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitSession);

	return true;
}

void UMainMenu::HostServer()
{
	
	if(MenuInterface != nullptr)
	{
		MenuInterface->Host();

	}

	//UE_LOG(LogTemp, Warning, TEXT("Host has been clicked!"));
}

void UMainMenu::JoinServer()
{
	if(MenuInterface != nullptr)
	{
		if(!ensure(IPAddressField!=nullptr)) return;
		const FString Address = IPAddressField->GetText().ToString();

		MenuInterface->Join(Address);
		//UE_LOG(LogTemp, Warning, TEXT("Joined the %s IP Address"), *Address);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if(!ensure(MenuSwitcher!=nullptr)) return;
	if(!ensure(JoinMenu!=nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if(!ensure(MenuSwitcher!=nullptr)) return;
	if(!ensure(MainMenu!=nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitSession()
{
	if(MenuInterface != nullptr)
	{
		MenuInterface->QuitGame();
	}
}
