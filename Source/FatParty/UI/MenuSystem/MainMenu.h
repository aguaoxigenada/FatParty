#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UEditableTextBox;
class UButton;
class UWidgetSwitcher;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
	
};

UCLASS()
class FATPARTY_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer &ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);
	FString HostName;

protected:
	virtual bool Initialize();

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
		UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		UButton* MenuHostButton;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CancelHostButton;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinIPButton;

	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
		UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* TextInput;

	//UPROPERTY(meta = (BindWidget))  Queda solo pa Hamachi
		//UEditableTextBox* IPAddressField;

	UFUNCTION()
		void OpenHostMenu();

	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void QuitSession();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();

};
