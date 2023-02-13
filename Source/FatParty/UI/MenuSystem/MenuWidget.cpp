#include "MenuWidget.h"

void UMenuWidget::Setup(bool IsHud)
{
	this->AddToViewport();

	if(!IsHud)
	{
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (!ensure(PlayerController != nullptr)) return;

		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(this->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);

		PlayerController->bShowMouseCursor = true;
	}
	
}

void UMenuWidget::Teardown()
{
	this->RemoveFromParent();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}

void UMenuWidget::SetMenuInterface(IMenuInterface* TheMenuInterface)
{
	MenuInterface = TheMenuInterface;
}
