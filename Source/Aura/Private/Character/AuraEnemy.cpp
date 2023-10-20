// Copyright Stas Company


#include "Character/AuraEnemy.h"
#include "DrawDebugHelpers.h"

void AAuraEnemy::HighlightActor()
{
	UE_LOG(LogTemp, Display, TEXT("%hs %s"), __FUNCTION__, *GetActorNameOrLabel());
	BP_HighlightActor();
}

void AAuraEnemy::UnHighlightActor()
{
	UE_LOG(LogTemp, Display, TEXT("%hs %s"), __FUNCTION__, *GetActorNameOrLabel());
	BP_UnHighlightActor();
}
