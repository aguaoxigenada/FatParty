#include "FatPartyGameMode.h"
#include "FatParty/Actors/DestructibleObject.h"
#include "Kismet/GameplayStatics.h"
#include "FatParty/Actors/Tower.h"
#include "FatParty/Characters/KnightCharacter.h"
#include "FatParty/Controllers/ThePlayerController.h"
#include "FatParty/UI/MenuSystem/NetworkErrorWidget.h"
#include "GameFramework/PlayerStart.h"
#include "AssetRegistryModule.h"

AFatPartyGameMode::AFatPartyGameMode()
{
    PlayerControllerClass = AThePlayerController::StaticClass();
    PopulatePlayerStartArray();
   
}

void AFatPartyGameMode::BeginPlay()
{
    Super::BeginPlay();

    
}

void AFatPartyGameMode::ActorDied(AActor *DeadActor)
{
    /* Condiciones de Victoria / Perder provisorias */

    // Se revisa desde el modo de juego si murio el personaje o si murieron todas las torretas

    if (AFatPartyCharacter* DestroyedPlayer = Cast<AFatPartyCharacter>(DeadActor))
    {
        DestroyedPlayer->HandleDestruction();
        if (PlayerController)
        {
            PlayerController->SetPlayerEnabledState(false);
        }
    }

    else if (AFatPartyEnemy* DestroyedEnemy = Cast<AFatPartyEnemy>(DeadActor))
    {
        DestroyedEnemy->HandleDestruction();

    	/*TargetTowers--;
        if(TargetTowers == 0)
        {
            GameOver(true);
        }*/    
    }
    else if(ADestructibleObject* DestroyedObject = Cast<ADestructibleObject>(DeadActor))
    {
	    DestroyedObject->HandleDestruction();
    }
}


UClass* AFatPartyGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    /* Override Functionality to get Pawn from PlayerController */
    AThePlayerController* MyController = Cast<AThePlayerController>(InController);
    if (MyController)
    {
        return MyController->GetPlayerPawnClass();
    }

    /* If we don't get the right Controller, use the Default Pawn */
    return DefaultPawnClass;

	///return Super::GetDefaultPawnClassForController_Implementation(InController);
}


void AFatPartyGameMode::HandleGameStart()
{
    // Calcula y retorna la cantidad de torres actual en el escenario
    TargetTowers = GetTargetTowerCount();
    FatPartyCharacter = Cast<AFatPartyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    PlayerController = Cast<AThePlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    // Llama al evento creado en el EventGraph
    StartGame();

    if (PlayerController)
    {
        PlayerController->SetPlayerEnabledState(false);

        // Se necesita para poder usar el FTimerDelegate
        FTimerHandle PlayerEnableTimerHandle;
        
        // El delegate permite crear un objeto, que puede anexar a una funcion directamente.
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            PlayerController,                                        // clase a utilizar
            &AThePlayerController::SetPlayerEnabledState,            // funcion de la clase
            true                                                     // parametro pasado a la clase
        );

        // Timer seteado con el tiempo del juego.
            GetWorldTimerManager().SetTimer(
                PlayerEnableTimerHandle, 
                PlayerEnableTimerDelegate,                           // Se llama despues del tiempo seteado en el StartDelay. 
                StartDelay, 
                false
        );
    }
}

int32 AFatPartyGameMode::GetTargetTowerCount()
{
    // Calcula y retorna la cantidad de Torres que hay actualmente en el escenario.
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}

AActor* AFatPartyGameMode::ChoosePlayerStart_Implementation(AController* Player)
{

    if (PlayerStartPoints.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, PlayerStartPoints.Num() - 1);
        return PlayerStartPoints[RandomIndex];
    }

    return nullptr;
}


void AFatPartyGameMode::PopulatePlayerStartArray()
{
    // Find all PlayerStart actors in the current level
    TArray<AActor*> PlayerStartActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartActors);

    // Loop through the found PlayerStart actors and add them to the PlayerStartArray
    for (AActor* Actor : PlayerStartActors)
    {
        APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
        if (PlayerStart)
        {
            PlayerStartPoints.Add(PlayerStart);
        	//UE_LOG(LogTemp, Warning, TEXT("Element %d"), PlayerStart);
        }
    }
}

void AFatPartyGameMode::GetAssetsInLevel(ULevel* TargetLevel, TArray<UObject*>& OutAssets)
{
    ULevel* LevelToQuery = TargetLevel; 
//	 ULevel* Level = UGameplayStatics::GetLevelByName(GetWorld(), LevelName);
    // Iterate through the actors in the level
    for (AActor* Actor : LevelToQuery->Actors)
    {
    	OutAssets.Add(Actor);
    }



 /*  
 
    // Get a reference to the Asset Registry module.
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    // Create a class filter to specify the asset class you want to retrieve (e.g., UStaticMesh::StaticClass()).
   // UClass* AssetClassFilter = UStaticMesh::StaticClass();  // Set to nullptr if you don't want to filter by class.

	ULevel* Level = GetLevel();

	//FString PackagePathString;
    FName PackagePathFilter;

    // Get the package name of the level.
    // Create a package path filter to specify the level's package path.
	//PackagePathString = Level->GetOutermost()->GetName();

	//PackagePathString = Level->GetOutermost()->GetName();
    //PackagePathFilter = FName(*PackagePathString);

     // Create a filter for combined level and class filtering.

    // El enfoque es al reves.  Hacemos que busqye la carpeta de los assets y vemos si estan anexados al nivel...



    FARFilter CombinedFilter;
  //  CombinedFilter.PackageNames.Add(LevelName);
    //CombinedFilter.ClassNames.Add(AssetClassFilter);
   // CombinedFilter.PackagePaths.Add(PackagePathFilter);
    //CombinedFilter.ClassPaths.Add();

    // Use the Asset Registry module to search for assets that match the filter.
    TArray<FAssetData> AssetData;  
    AssetRegistryModule.Get().GetAssets(CombinedFilter, AssetData);

	// Iterate through the results and add assets to the OutAssets array.
   for (const FAssetData& Asset : AssetData)
    {
        // Get a reference to the asset.
        UObject* AssetObject = Asset.GetAsset();

		AssetObject->CollectDefaultSubobjects(OutAssets);
        // Check if the asset is valid before adding it to the output array.
}
    
    /*
	
  
    */
/*
    UWorld* World = GetWorld();
    // Create a filter to specify the level name.
    FString LevelNameFilter = LevelName.ToString();

   


   	/*if (AssetObject)
        {
			ULevel* LevelToQuery = TargetLevel;  //
           // World->GetLevel();

        	// Iterate through the actors in the level
		    for (AActor* Actor : LevelToQuery->Actors)
		    {
		        // Check if Actor is a UClass you're interested in (e.g., UStaticMesh)
		        if (Actor->IsA(UStaticMesh::StaticClass()))
		        {
		            UStaticMesh* StaticMeshActor = Cast<UStaticMesh>(Actor);

		            if (StaticMeshActor)
		            {
		            //    UStaticMesh* Mesh = StaticMeshActor/*->GetStaticMeshComponent()->GetStaticMesh()*/;
                        // OutAssets.Add(StaticMeshActor);
		              /*  if (Mesh)
		                {
		                   OutAssets.Add(Mesh);
		                }
		                */
		    //        }
		      //  }
		  //  }
       // }
    //}*/

}

/* USAMOS ESTO PERO TOCA HACER ASINCRONA EL CAMBIO DE NIVEL...
 * Del lobby al nivel,
 * Del lobby siempre al nivel...
ULevel* LevelToQuery = TargetLevel; 
//	 ULevel* Level = UGameplayStatics::GetLevelByName(GetWorld(), LevelName);
    // Iterate through the actors in the level
    for (AActor* Actor : LevelToQuery->Actors)
    {
    	OutAssets.Add(Actor);
    }

  */ 