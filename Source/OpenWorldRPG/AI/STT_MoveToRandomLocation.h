#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "NavigationSystem.h"

#include "STT_MoveToRandomLocation.generated.h"

USTRUCT()
struct FSTT_MoveToRandomLocationInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	AActor* SelfActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	FVector TargetLocation = FVector::ZeroVector;
};

USTRUCT(DisplayName = "Move To Random Location")
struct FSTT_MoveToRandomLocation: public FStateTreeTaskBase
{
	GENERATED_BODY()

	typedef FSTT_MoveToRandomLocationInstanceData FInstanceDataType;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct();}
	virtual bool Link(FStateTreeLinker& Linker) override{ return true; }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;


	
};
