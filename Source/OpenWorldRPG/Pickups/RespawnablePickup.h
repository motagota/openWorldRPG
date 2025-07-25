#pragma once
#include "../InteractableInterface.h"
#include "../Items/ItemBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnablePickup.generated.h"


class USphereComponent;
class UWidgetComponent;
class UStaticMeshComponent;
class UInventoryComponent;

UCLASS()

class OPENWORLDRPG_API ARespawnablePickup : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ARespawnablePickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* PickupWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	TSubclassOf<UItemBase> ItemToGiveClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 30.f;

private:
	bool bIsAvailable = true;
	FTimerHandle RespawnTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Respawn();
	void Interact_Implementation(AActor* Interactor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void TryPickup(AActor* Picker);
};
