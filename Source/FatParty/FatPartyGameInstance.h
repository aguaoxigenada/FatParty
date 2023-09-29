#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/HudWidget.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UI/MenuSystem/MenuInterface.h"
#include "UI/MenuSystem/MainMenu.h"
#include "FatPartyGameInstance.generated.h"

class UMenuWidget;
class UMainMenu;


UCLASS()
class FATPARTY_API UFatPartyGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	
public:
	UFatPartyGameInstance(const FObjectInitializer &ObjectInitializer);
	virtual void Init() override;

	virtual void QuitGame() override;

	UMenuWidget* GetInGameMenu() const;

	UFUNCTION(Exec, BlueprintCallable)
		void LoadMenuWidget();

	UFUNCTION(Exec, BlueprintCallable)
		void LoadInGameMenu();

	UFUNCTION(Exec, BlueprintCallable)
		void LoadTimer();
	
	UFUNCTION(Exec, BlueprintCallable)
		void LoadHUD();

	UFUNCTION(Exec)
		virtual void Host(FString ServerName) override;

	UFUNCTION()
		virtual void LoadGameMenu() override;

	UFUNCTION()
		virtual void LoadNextLevel() override;

	UFUNCTION()
		virtual void RestartLevel() override;

	UFUNCTION()
		virtual void PlayerRetry() override;
//	FOnSteamAuthFailureDelegate OnOverrideFailure;	
	//UFUNCTION(Exec)  Lo voy a usar solo para Hamachi
		//virtual void Join(const FString& Address) override;

	UFUNCTION(Exec)
		virtual void Join(uint32 Index) override;

	void RefreshServerList() override;

	IOnlineSessionPtr SessionInterface;

	UMenuWidget* PlayerHud;

	void NetworkErrorPopUp();

	void LoadingWBP();

	TSubclassOf<class UUserWidget> LoadingClass;
	TSubclassOf<class UUserWidget>TimerClass;
private:

	TSubclassOf<class UUserWidget> NetworkErrorClass;
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	TSubclassOf<class UUserWidget> HudClass;
	

	UMainMenu* Menu;
	UUserWidget* LoadingScreen;
	UMenuWidget* InGameMenu;
	UMenuWidget* NetworkError;
	UMenuWidget* TimerWidget;

	FString DesiredServerName;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void CreateSession();

};