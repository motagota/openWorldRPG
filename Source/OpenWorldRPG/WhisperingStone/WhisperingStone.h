#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "OpenWorldRPG/OpenWorldRPGPlayerController.h"
#include "WhisperingStone.generated.h"

UCLASS()
class OPENWORLDRPG_API AWhisperingStone : public AActor
{
	GENERATED_BODY()
	
public:	
	AWhisperingStone();

	UPROPERTY(EditAnywhere, Category = "WhisperingStone")
	UWidgetComponent* InteractionPrompt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WhisperingStone")
	FName WhisperingStoneID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WhisperingStone")
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WhisperingStone")
	class UStaticMeshComponent* Mesh;

	// Widget classes for tutorial and fast travel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> TutorialWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> FastTravelWidgetClass;

	bool bPlayerInRange = false;
	bool bHasBeenUsed = false;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "WhisperingStone")
	void OnTouchedByPlayer(class AOpenWorldRPGPlayerController* player);
	
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	void NotifyActorEndOverlap(AActor* OtherActor);

private:
	// Helper function to show widgets
	void ShowTutorialWidget(AOpenWorldRPGPlayerController* PlayerController);
	void ShowFastTravelWidget(AOpenWorldRPGPlayerController* PlayerController);
};
