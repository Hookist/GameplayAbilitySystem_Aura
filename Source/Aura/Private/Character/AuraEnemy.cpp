// Copyright Stas Company


#include "Character/AuraEnemy.h"
#include "DrawDebugHelpers.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::HighlightActor()
{
	UE_LOG(LogTemp, Display, TEXT("%hs %s"), __FUNCTION__, *GetActorNameOrLabel());
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
	BP_HighlightActor();
}

void AAuraEnemy::UnHighlightActor()
{
	UE_LOG(LogTemp, Display, TEXT("%hs %s"), __FUNCTION__, *GetActorNameOrLabel());
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
	BP_UnHighlightActor();
}
