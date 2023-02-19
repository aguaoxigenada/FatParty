#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

class UWidgetSwitcher;
class UButton;

UCLASS()
class FATPARTY_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	void EndLevelScreen(bool bWon);

protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
		UButton* MenuButton;

	UPROPERTY(meta = (BindWidget))
		UButton* LevelPassed_MenuButton;

	UPROPERTY(meta = (BindWidget))
		UButton* LevelLost_MenuButton;

	UPROPERTY(meta = (BindWidget))
		UButton* NextLevelButton;

	UPROPERTY(meta = (BindWidget))
		UButton* RetryButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* InGameMenuSwitcher;
	
	UPROPERTY(meta = (BindWidget))
		UWidget* InGameMenu;

	UPROPERTY(meta = (BindWidget))
		UWidget* LevelPassed;

	UPROPERTY(meta = (BindWidget))
		UWidget* LevelLost;

	UPROPERTY(meta = (BindWidget))
		UWidget* PauseMenu;

	UFUNCTION()
		void GoToMainMenu();

	UFUNCTION()
		void GoToNextLevel();

	UFUNCTION()
		void RetryLevel();

	UFUNCTION()
		void CancelMenu();

	APlayerController* PlayerController;
};
