
#include "DataFragmentPickup.h"

#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "OpenWorldRPG/OpenWorldRPGCharacter.h"

ADataFragmentPickup::ADataFragmentPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(50.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("Trigger"));
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bCollisionEnabled = false;
	RootComponent = CollisionSphere;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

void ADataFragmentPickup::BeginPlay()
{
	Super::BeginPlay();

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0, 0, 1000.0f); 

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); 

	bool bHitGround = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility, 
		CollisionParams
	);

	if (bHitGround)
	{		
		FVector NewLocation = HitResult.ImpactPoint + FVector(0, 0, HeightAboveGround);
		SetActorLocation(NewLocation);
	}
	
	InitialLocation = GetActorLocation();
}

void ADataFragmentPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0, RotationSpeed* DeltaTime, 0));

	float BobOffset = FMath::Sin(GetWorld()->GetTimeSeconds()*BobSpeed) * BobHeight;
	SetActorLocation(InitialLocation +FVector(0,0, BobOffset));
}

void ADataFragmentPickup::EnableCollision()
{
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->OnComponentBeginOverlap.RemoveDynamic(this, &ADataFragmentPickup::OnOverLapBegin);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADataFragmentPickup::OnOverLapBegin);
	bCollisionEnabled = true;
}

void ADataFragmentPickup::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("OnOverLapBegin"));
	
	if (!bCollisionEnabled || !OtherActor) return;
    
	if ( UAbilitySystemComponent* ASC = OtherActor->FindComponentByClass<UAbilitySystemComponent>() )
	{
		UE_LOG(LogTemp, Display, TEXT("OnOverLapBegin: ASC"));


			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DataFragmentEffect, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				UE_LOG(LogTemp, Display, TEXT("OnOverLapBegin: Spec valid %d"), FragmentValue);
				SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Fragment.Amount")), FragmentValue);
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}			
			
				
	}
	Destroy();
	
}
