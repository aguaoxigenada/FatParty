#include "FatPartyGameInstance.h"
#include "Engine/WorldComposition.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
//#include "PlatformTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"


UFatPartyGameInstance::UFatPartyGameInstance(const FObjectInitializer &ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget>MainMenuBP_Class(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));
	if(!ensure(MainMenuBP_Class.Class!=nullptr)) return;
	
	ConstructorHelpers::FClassFinder<UUserWidget>InGameMainMenuBP_Class(TEXT("/Game/Blueprints/UI/WBP_InGameMenu"));
	if(!ensure(InGameMainMenuBP_Class.Class!=nullptr)) return;

	MenuClass = MainMenuBP_Class.Class;
	InGameMenuClass = InGameMainMenuBP_Class.Class;	
}

void UFatPartyGameInstance::Init()  // Es necesario
{
	Super::Init();

	//UE_LOG(LogTemp, Warning, TEXT("Found Class %s"), *MenuClass->GetName());
}


void UFatPartyGameInstance::LoadMenu()
{
	if(!ensure(MenuClass !=nullptr)) return;

	Menu = CreateWidget<UMenuWidget>(this, MenuClass);
	if(!ensure(Menu !=nullptr)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);  // esta ok el this, porque esta clase implementa la IMenuInterface
}

void UFatPartyGameInstance::LoadInGameMenu()
{
	if(!ensure(InGameMenuClass !=nullptr)) return;

	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if(!ensure(InGameMenu !=nullptr)) return;

	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
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


	World->ServerTravel("/Game/Maps/Dungeon_01?listen?ip=25.14.116.251");  // aca iria lo de hamachi si lo quiero hacer listener 192.168.1.10   25.14.116.251
}

void UFatPartyGameInstance::LoadGameMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController!=nullptr)) return;
	
	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UFatPartyGameInstance::RestartLevel()
{
	UWorld* World = GetWorld();
	if(!ensure(World!=nullptr)) return;

	FString LevelURL = GetWorld()->GetAddressURL();

	World->ServerTravel(LevelURL, ETravelType::TRAVEL_Absolute);
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
