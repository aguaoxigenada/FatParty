#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/HudWidget.h"
#include "UI/MenuSystem/MenuInterface.h"
#include "FatPartyGameInstance.generated.h"

class UMenuWidget;

UCLASS()
class FATPARTY_API UFatPartyGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UFatPartyGameInstance(const FObjectInitializer &ObjectInitializer);
	virtual void Init();

	virtual void QuitGame() override;

	UMenuWidget* GetInGameMenu() const;

	UFUNCTION(Exec, BlueprintCallable)
		void LoadMenu();

	UFUNCTION(Exec, BlueprintCallable)
		void LoadInGameMenu();

	UFUNCTION(Exec, BlueprintCallable)
		void LoadHUD();

	UFUNCTION(Exec)
		virtual void Host() override;

	UFUNCTION()
		virtual void LoadGameMenu() override;

	UFUNCTION()
		virtual void LoadNextLevel() override;

	UFUNCTION()
		virtual void RestartLevel() override;

	UFUNCTION(Exec)
		virtual void Join(const FString& Address) override;

	UMenuWidget* PlayerHud;

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	TSubclassOf<class UUserWidget> HudClass;

	UMenuWidget* Menu;
	UMenuWidget* InGameMenu;
	
};
