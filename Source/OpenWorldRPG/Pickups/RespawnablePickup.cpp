// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnablePickup.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Actor/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ARespawnablePickup::ARespawnablePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

	PickupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidgetComponent"));
	PickupWidgetComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ARespawnablePickup::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARespawnablePickup::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ARespawnablePickup::OnSphereEndOverlap);

	PickupWidgetComponent->SetVisibility(false);
	
}

// Called every frame
void ARespawnablePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARespawnablePickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsAvailable)
	{
		PickupWidgetComponent->SetVisibility(true);
	}
}

void ARespawnablePickup::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PickupWidgetComponent->SetVisibility(false);
}

void ARespawnablePickup::TryPickup(AActor* Picker)
{
	UE_LOG(LogTemp, Display, TEXT("TryPickup"));
	if (!bIsAvailable || !ItemToGiveClass  || Quantity <= 0) return;

	UItemBase* ItemInstance = NewObject<UItemBase>(this, ItemToGiveClass);
	if (!ItemInstance) return;
	
	UInventoryComponent* Inventory = Picker->FindComponentByClass<UInventoryComponent>();
	if (Inventory && Inventory->AddItem(ItemInstance, Quantity))
	{
		MeshComponent->SetVisibility(false, true);
		PickupWidgetComponent->SetVisibility(false);
		bIsAvailable = false;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ARespawnablePickup::Respawn, RespawnTime, false);
	}
}

void ARespawnablePickup::Respawn()
{
	MeshComponent->SetVisibility(true, true);
	bIsAvailable = true;
}

void ARespawnablePickup::Interact_Implementation(AActor* Interactor)
{
	IInteractableInterface::Interact_Implementation(Interactor);
	
	TryPickup(Interactor);
}

