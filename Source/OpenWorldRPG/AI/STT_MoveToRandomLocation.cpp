#include "STT_MoveToRandomLocation.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSTT_MoveToRandomLocation::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData<FInstanceDataType>(*this);
	
	if (!InstanceData.SelfActor)
	{
		return EStateTreeRunStatus::Failed;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(InstanceData.SelfActor->GetWorld());
	if (!NavSys)
	{
		return EStateTreeRunStatus::Failed;
	}

	FNavLocation RandomPoint;
	const bool bFound = NavSys->GetRandomPointInNavigableRadius(InstanceData.SelfActor->GetActorLocation(), InstanceData.Radius, RandomPoint);
	if (!bFound)
	{
		return EStateTreeRunStatus::Failed;
	}

	// Output the random point
	InstanceData.TargetLocation = RandomPoint.Location;

	return EStateTreeRunStatus::Succeeded;
}