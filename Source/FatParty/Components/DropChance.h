#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dropchance.generated.h"

class ADestructibleObject;
class ABasePowerUp;

USTRUCT(BlueprintType)
struct FDLootTable
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TMap<int32, int32> RangeChances;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		TArray<TSubclassOf<ABasePowerUp>> Items;
	
	UPROPERTY(EditDefaultsOnly)
		int32 MaxRange;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FATPARTY_API UDropChance : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDropChance();

	UPROPERTY(EditDefaultsOnly)
		FDLootTable LootTable;

	UFUNCTION()
		void PickItem(ABasePowerUp* PickedItem);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		int32 RandomNumber;
	
	ADestructibleObject* WorldObject;

			
};
