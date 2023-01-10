#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UEditableTextBox;
class UButton;
class UWidgetSwitcher;

UCLASS()
class FATPARTY_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
		UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton;

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
		UEditableTextBox* IPAddressField;

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

};
