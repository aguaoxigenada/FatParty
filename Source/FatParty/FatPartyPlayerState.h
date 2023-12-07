// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FatPartyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FATPARTY_API AFatPartyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, Category = "FatParyPlayerState")
		TSubclassOf<class AFatPartyCharacter> SelectedPawnClass;

	// Function to set the selected pawn class
    UFUNCTION(BlueprintCallable, Category = "FatParyPlayerState")
		void SetSelectedPawnClass(TSubclassOf<AFatPartyCharacter> NewSelectedClass);

	    // Declare the function prototype
    UFUNCTION(BlueprintCallable, Category = "YourCategory")
		static FString GenerateUUID();

	UFUNCTION(BlueprintCallable)
		void SetPlayerID();

	UFUNCTION(BlueprintCallable)
		FString GetPlayerID();

	UPROPERTY(Replicated)
		FString PlayerGUID;

	//UFUNCTION(BlueprintCallable, Category = "FatParyPlayerState")
	 //AFatPartyCharacter PawnChoosen;
};
