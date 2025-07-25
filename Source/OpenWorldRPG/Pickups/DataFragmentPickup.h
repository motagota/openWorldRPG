#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "DataFragmentPickup.generated.h"

UCLASS()
class OPENWORLDRPG_API ADataFragmentPickup : public AActor
{
	GENERATED_BODY()	
public:	
	ADataFragmentPickup();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataFragments")
	int32 FragmentValue =10;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Pickup Settings")
	float HeightAboveGround = 30.0f; 

	FVector InitialLocation; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DataFragments")
	TSubclassOf<UGameplayEffect> DataFragmentEffect;
	
	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="DataFragments")
	void EnableCollision();

private:
	float RotationSpeed = 90.0f;
	float BobSpeed = 2.0f;
	float BobHeight = 20.0f;
	
	bool bCollisionEnabled;
};




