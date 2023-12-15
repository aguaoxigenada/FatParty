#include "DropChance.h"
#include "FatParty/Actors/PowerUps/BasePowerUp.h"
#include "FatParty/Actors/DestructibleObject.h"


UDropChance::UDropChance()
{
	WorldObject = Cast<ADestructibleObject>(this->GetOwner());
}

void UDropChance::BeginPlay()
{
	Super::BeginPlay();

	RandomNumber = FMath::RandRange(1, LootTable.MaxRange);
	//UE_LOG(LogTemp, Warning, TEXT("The RandomNumber is: %d"), RandomNumber);
	
	int32 ArrayNum = 0;

	for (const TPair<int32, int32> RandomItem : LootTable.RangeChances)
	{
		if(RandomNumber >= RandomItem.Key && RandomNumber <= RandomItem.Value)
		{
			if(LootTable.Items.IsEmpty()) return;
			PickItem(LootTable.Items[ArrayNum].GetDefaultObject());		
		}

		++ArrayNum;
	}
}

void UDropChance::PickItem(ABasePowerUp* PickedItem)
{
	WorldObject->SetObjectToDrop(PickedItem);
}


