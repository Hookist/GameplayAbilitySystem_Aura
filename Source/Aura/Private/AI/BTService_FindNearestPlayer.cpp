// Copyright Stas Company


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* owningPawn = AIOwner->GetPawn();

	const FName targetTag = owningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> actorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(owningPawn, targetTag, actorsWithTag);
}
