#include "ServerRow.h"
#include "Components/Button.h"
#include "MainMenu.h"

void UServerRow::Setup(UMainMenu* Parent, uint32 Index)
{
	MainMenu = Parent;
	RowIndex = Index;

	if(!ensure(ServerRowButton!=nullptr)) return;
	ServerRowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}


void UServerRow::OnClicked()
{
	MainMenu->SelectIndex(RowIndex);
}

