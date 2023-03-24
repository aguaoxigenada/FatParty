#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

UCLASS()
class FATPARTY_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta =(BindWidget))
	class UTextBlock* ServerName;
	
	UPROPERTY(meta =(BindWidget))
		class UTextBlock* HostUsername;

	UPROPERTY(meta =(BindWidget))
		class UTextBlock* ConnectionFraction;
	
	UPROPERTY(BlueprintReadOnly)
		bool Selected = false;

	void Setup(class UMainMenu* Parent, uint32 Index);

private:
	UPROPERTY(meta =(BindWidget))
	class UButton* ServerRowButton;

	UFUNCTION()
		void OnClicked();

	UMainMenu* MainMenu;
	uint32 RowIndex;
	
};
