#include "FatPartyGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI/HudWidget.h"
#include "UI/MenuSystem/InGameMenu.h"


UFatPartyGameInstance::UFatPartyGameInstance(const FObjectInitializer &ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget>MainMenuBP_Class(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));
	if(!ensure(MainMenuBP_Class.Class!=nullptr)) return;
	
	ConstructorHelpers::FClassFinder<UUserWidget>InGameMainMenuBP_Class(TEXT("/Game/Blueprints/UI/WBP_InGameMenu"));
	if(!ensure(InGameMainMenuBP_Class.Class!=nullptr)) return;

	ConstructorHelpers::FClassFinder<UUserWidget>InGameHudBP_Class(TEXT("/Game/Blueprints/UI/WBP_HUD"));
	if(!ensure(InGameHudBP_Class.Class!=nullptr)) return;

	MenuClass = MainMenuBP_Class.Class;
	InGameMenuClass = InGameMainMenuBP_Class.Class;
	HudClass = InGameHudBP_Class.Class;
}

void UFatPartyGameInstance::Init()  
{
	Super::Init();

}


void UFatPartyGameInstance::LoadMenu()
{
	if(!ensure(MenuClass !=nullptr)) return;

	Menu = CreateWidget<UMenuWidget>(this, MenuClass);
	if(!ensure(Menu !=nullptr)) return;

	Menu->Setup(false);
	Menu->SetMenuInterface(this);  //Implementa la IMenuInterface
}

void UFatPartyGameInstance::LoadInGameMenu()
{
	if(!ensure(InGameMenuClass !=nullptr)) return;

	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if(!ensure(InGameMenu !=nullptr)) return;
	
	InGameMenu->Setup(false);
	InGameMenu->SetMenuInterface(this);
}

void UFatPartyGameInstance::LoadHUD()
{
	if(!ensure(HudClass !=nullptr)) return;
	
	PlayerHud = CreateWidget<UHudWidget>(this, HudClass);
	if(!ensure(HudClass !=nullptr)) return;
	
	PlayerHud->Setup(true);
	PlayerHud->SetMenuInterface(this);
}

void UFatPartyGameInstance::Host()
{
	if(Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;

	// Aca se coloca la URL de Hamachi o propia para jugar en LAN
	World->ServerTravel("/Game/Maps/Dungeon_01?listen?ip=25.76.209.200");  

}

void UFatPartyGameInstance::LoadGameMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController!=nullptr)) return;
	
	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);

	if(PlayerHud != nullptr)
	{
		PlayerHud->Teardown();
	}
}

void UFatPartyGameInstance::LoadNextLevel()
{
	UWorld* World = GetWorld();
	if(!ensure(World!=nullptr)) return;
	
	if(PlayerHud != nullptr)
	{
		PlayerHud->Teardown();
	}

	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Travelling to new Level"));

	World->ServerTravel("/Game/Maps/Dungeon_02", ETravelType::TRAVEL_Absolute);
}

void UFatPartyGameInstance::RestartLevel()
{
	UWorld* World = GetWorld();
	if(!ensure(World!=nullptr)) return;

	FString LevelURL = GetWorld()->GetAddressURL();

	// Esta OK porque el server va a ser Dedicado.  Entonces todos los players que entran son Clientes.
	World->GetFirstPlayerController()->ClientTravel(LevelURL, ETravelType::TRAVEL_Absolute);
	//World->ServerTravel(LevelURL, ETravelType::TRAVEL_Absolute);
}

void UFatPartyGameInstance::QuitGame()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController!=nullptr)) return;
	
	PlayerController->ConsoleCommand("quit");
}

UMenuWidget* UFatPartyGameInstance::GetInGameMenu() const
{
	return InGameMenu;
}

void UFatPartyGameInstance::Join(const FString& Address)
{
	if(Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController!=nullptr)) return;
	
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
