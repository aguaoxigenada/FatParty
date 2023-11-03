#include "FatPartyGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Online.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Controllers/ThePlayerController.h"
#include "GameMode/FatPartyGameMode.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UI/HudWidget.h"
#include "UI/TimerWidget.h"
#include "UI/MenuSystem/InGameMenu.h"
#include "UI/MenuSystem/NetworkErrorWidget.h"
#include "UI/MenuSystem/MainMenu.h"


const static FName SESSION_NAME = NAME_GameSession;
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");


UFatPartyGameInstance::UFatPartyGameInstance(const FObjectInitializer &ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget>MainMenuBP_Class(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));
	if(!ensure(MainMenuBP_Class.Class!=nullptr)) return;
	
	ConstructorHelpers::FClassFinder<UUserWidget>InGameMainMenuBP_Class(TEXT("/Game/Blueprints/UI/WBP_InGameMenu"));
	if(!ensure(InGameMainMenuBP_Class.Class!=nullptr)) return;

	ConstructorHelpers::FClassFinder<UUserWidget>InGameHudBP_Class(TEXT("/Game/Blueprints/UI/WBP_HUD"));
	if(!ensure(InGameHudBP_Class.Class!=nullptr)) return;

	ConstructorHelpers::FClassFinder<UUserWidget>NetworkErrorBP_Class(TEXT("/Game/Blueprints/UI/WBP_NetWorkError"));
	if(!ensure(NetworkErrorBP_Class.Class!=nullptr)) return;

	ConstructorHelpers::FClassFinder<UUserWidget>LoadingBP_Class(TEXT("/Game/Blueprints/UI/WBP_Loading"));
	if(!ensure(LoadingBP_Class.Class!=nullptr)) return;

	ConstructorHelpers::FClassFinder<UUserWidget>GameTimerBP_Class(TEXT("/Game/Blueprints/UI/WBP_GameTimer"));
	if(!ensure(GameTimerBP_Class.Class!=nullptr)) return;

	MenuClass = MainMenuBP_Class.Class;
	InGameMenuClass = InGameMainMenuBP_Class.Class;
	NetworkErrorClass = NetworkErrorBP_Class.Class;
	HudClass = InGameHudBP_Class.Class;
	LoadingClass = LoadingBP_Class.Class;
	TimerClass = GameTimerBP_Class.Class;


	
}

void UFatPartyGameInstance::Init()  
{
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if(Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();

		if(SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UFatPartyGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UFatPartyGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UFatPartyGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UFatPartyGameInstance::OnJoinSessionComplete);
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}

	if(GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UFatPartyGameInstance::OnNetworkFailure);
	}

	


}

void UFatPartyGameInstance::CreateSession()
{
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		SessionSettings.NumPublicConnections = 3; // esto lo puedo cambiar
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
	}

	/* Mi Forma
	 if(SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		FOnlineSessionSetting  SessionNameSettings;

		SessionNameSettings.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineServiceAndPing;
		SessionNameSettings.Data = Menu->HostName;
		
		if(IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;  // para que no sea necsario mandar un invite.
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(TEXT("Test"), FString("Hello"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionSettings.Settings.Add(FName("SESSION_NAME"), SessionNameSettings);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
	 */
}



void UFatPartyGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if(bWasSuccessful && SessionSearch.IsValid() && Menu != nullptr)
	{
		TArray Results = SessionSearch->SearchResults;
		TArray<FServerData>ServerNames;
		/*
		Se pueden agregar mas para testear.
		ServerNames.Add("Test Server 1");
		*/

		UE_LOG(LogTemp, Warning, TEXT("finished find session"));
		for (const FOnlineSessionSearchResult& SearchResult : Results)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;   //NumOpenPublicConnections son las disponibles.
																									 // Esto funciona bien desde Steam.
			Data.HostUsername = SearchResult.Session.OwningUserName;

			//Esta es de mi intento TB ¡Funciona!
			//Data.Name = SearchResult.Session.SessionSettings.Settings.FindRef("SESSION_NAME").Data.ToString();

			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}

			ServerNames.Add(Data);
		}
		Menu->SetServerList(ServerNames);
	}
	
}

void UFatPartyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(!SessionInterface.IsValid()) return;

	FString TravelURL;                                   // Es un Out Object
	if(!SessionInterface->GetResolvedConnectString(SessionName, TravelURL))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could Not Connect"));
		return;
	}

	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;

	//Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *TravelURL));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController!=nullptr)) return;

	PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);

}

void UFatPartyGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if(!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session has already been created"));
		return;
	}

	if(Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;

	//Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/Lobby?listen"); 
}

void UFatPartyGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if(Success)
	{
		CreateSession();
	}
}

void UFatPartyGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver,
	ENetworkFailure::Type FailureType, const FString& ErrorString)
{

	NetworkErrorPopUp();
	FString ErrorMessage = FString::Printf(TEXT("Network Failure (%s): %s"), ENetworkFailure::ToString(FailureType), *ErrorString);

	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Red, ErrorMessage);

	
}

void UFatPartyGameInstance::NetworkErrorPopUp()
{
	UE_LOG(LogTemp, Warning, TEXT("IT WORKED"));

	if(!ensure(NetworkErrorClass !=nullptr)) return;

	NetworkError = CreateWidget<UNetworkErrorWidget>(this, NetworkErrorClass);
	if(!ensure(NetworkError !=nullptr)) return;

	NetworkError->Setup(false);
	NetworkError->SetMenuInterface(this);

}

void UFatPartyGameInstance::LoadTimer()
{
	if(!ensure(TimerClass !=nullptr)) return;

	TimerWidget = CreateWidget<UTimerWidget>(this, TimerClass);
	if(!ensure(TimerWidget !=nullptr)) return;

	TimerWidget->Setup(true);
	TimerWidget->SetMenuInterface(this);  
}


void UFatPartyGameInstance::LoadMenuWidget()
{
	if(!ensure(MenuClass !=nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
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
	AFatPartyGameMode* FatPartyGameMode = Cast<AFatPartyGameMode>(GetWorld()->GetAuthGameMode());
	AThePlayerController* PlayerController = Cast<AThePlayerController>(GetFirstLocalPlayerController());
	// Parece que esto tiene que ser una funcion aparte y el game mode la puede llamar para todos...
	if(PlayerHud != nullptr)
	{
		PlayerHud->Teardown();
	}

		UWorld* World = GetWorld();

    if (World)
    {
        // Check the network mode using IsNetMode
        if (World->IsNetMode(NM_DedicatedServer))
        {
            UE_LOG(LogTemp, Warning, TEXT("Running on a dedicated server"));
        }
        else if (World->IsNetMode(NM_ListenServer))
        {
          	FatPartyGameMode->TriggerNextLevel();
        }
        else if (World->IsNetMode(NM_Standalone))
        {
            UE_LOG(LogTemp, Warning, TEXT("Running in standalone mode"));
        }
        else if (World->IsNetMode(NM_Client))
        {
			//FatPartyGameMode->TriggerNextLevel();
			PlayerController->SendToNextLevelClientCall();
        }
    }

	
}

void UFatPartyGameInstance::LoadNextLevelMulticast_Implementation()
{
	// Borrar esto....
	// Me parece que esto tiene que hacerse en el game mode...
	UWorld* World = GetWorld();
	if(!ensure(World!=nullptr)) return;
	
	if(PlayerHud != nullptr)
	{
		PlayerHud->Teardown();
	}

	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Travelling to new Level"));

	// hay que ver de hacerlo seamless, y en el caso de que sea el cliente el que apreta el next level???

	//bUseSeamlessTravel = true;
	//World->ServerTravel;
	
	World->SeamlessTravel("/Game/Maps/Level_02/Dungeon_02", ETravelType::TRAVEL_Absolute);

	// SE tiene que hacer un case para cualquier posible nivel que entre en el juego.
}

void UFatPartyGameInstance::RestartLevel() 
{
	UWorld* World = GetWorld();
	if(!ensure(World!=nullptr)) return;

	FString LevelURL = GetWorld()->GetAddressURL();

	// Esta OK porque el server va a ser Dedicado.  Entonces todos los players que entran son Clientes.  NO necesariamente...

	//World->ServerTrav(LevelURL, ETravelType::TRAVEL_Absolute);
	World->GetFirstPlayerController()->ClientTravel(LevelURL, ETravelType::TRAVEL_Absolute);
}

void UFatPartyGameInstance::PlayerRetry() 
{
	AThePlayerController* PlayerController = Cast<AThePlayerController>(GetFirstLocalPlayerController());
	AController* TestPlayerController = PlayerController;
	UWorld* World = GetWorld();

    if (World)
    {
        // Check the network mode using IsNetMode
        if (World->IsNetMode(NM_DedicatedServer))
        {
            // This code runs on a dedicated server
            UE_LOG(LogTemp, Warning, TEXT("Running on a dedicated server"));
        }
        else if (World->IsNetMode(NM_ListenServer))
        {
          	// Handle server-side respawn logic
			 UE_LOG(LogTemp, Warning, TEXT("Running on a Listener server"));
			PlayerController->RespawnPlayer(TestPlayerController);
			
        }
        else if (World->IsNetMode(NM_Standalone))
        {
            // This code runs in a standalone game (not networked)
            UE_LOG(LogTemp, Warning, TEXT("Running in standalone mode"));
        }
        else if (World->IsNetMode(NM_Client))
        {
            // Call the server to request respawn
			PlayerController->ServerRespawnPlayer(TestPlayerController);
			 UE_LOG(LogTemp, Warning, TEXT("Running on a Client"));
        }
    }

	
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

void UFatPartyGameInstance::Join(uint32 Index)
{
	/*  El Address ya no lo recibe, podria hacerse otra funcion en la que si se utilice.
	 
	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController!=nullptr)) return;
	
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	*/

	if(!SessionInterface.IsValid()) return;
	if(!SessionSearch.IsValid()) return;

	if(Menu != nullptr)
	{
		//Menu->SetServerList({"Test1", "Test2"});  Para Testing
		Menu->Teardown();
	}

	if(SessionSearch->SearchResults[Index].Session.SessionSettings.NumPublicConnections <= 0)
	{
		return;
	}

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);

}

void UFatPartyGameInstance::Host(FString ServerName)  // revisar
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
		if (ExistingSession != nullptr) 
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}
		else
		{
			CreateSession();
		}
	}

	/*OLD FOR HAMACHI
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
	World->ServerTravel("/Game/Maps/Dungeon_02?listen?ip=25.5.193.208");  

	 */
	
}

void UFatPartyGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if(SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting to look for sessions"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}


void UFatPartyGameInstance::LoadingWBP()
{

	if(!ensure(LoadingClass !=nullptr)) return;

	LoadingScreen = CreateWidget<UUserWidget>(this, LoadingClass);
	if(!ensure(LoadingScreen !=nullptr)) return;

	LoadingScreen->AddToViewport();

}


