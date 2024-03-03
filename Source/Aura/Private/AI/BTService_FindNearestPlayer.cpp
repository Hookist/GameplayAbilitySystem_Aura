// Copyright Stas Company


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* owningPawn = AIOwner->GetPawn();

	const FName targetTag = owningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> actorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(owningPawn, targetTag, actorsWithTag);

	float closestDistance = TNumericLimits<float>::Max();
	AActor* closestActor = nullptr;
	for (AActor* actor : actorsWithTag)
	{
		GEngine->AddOnScreenDebugMessage(2, .5f, FColor::Orange, *actor->GetName());

		if (IsValid(actor) && IsValid(owningPawn))
		{
			const float distance = owningPawn->GetDistanceTo(actor);
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestActor = actor;
			}
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, closestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, closestDistance);
}
