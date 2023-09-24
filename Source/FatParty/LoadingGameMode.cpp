#include "LoadingGameMode.h"

void ALoadingGameMode::TravelToLevel()
{
	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Dungeon_01?listen");
}
