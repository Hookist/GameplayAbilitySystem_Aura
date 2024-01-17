// Copyright Stas Company


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* myObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return myObj;
}

void UTargetDataUnderMouse::Activate()
{
	FVector hitLocation;

	APlayerController* pawnPlayerController = Ability->GetActorInfo().PlayerController.Get();
	FHitResult cursorHit;
	pawnPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
	hitLocation = cursorHit.Location;
	ValidData.Broadcast(hitLocation);
	// AActor* avatarActor = AbilitySystemComponent.Get()->GetAvatarActor();
	// if (!avatarActor)
	// 	return;
	//
	// APawn* avatarPawn = Cast<APawn>(avatarActor);
	// if (!avatarPawn)
	// 	return;
	//
	// AController* pawnController = avatarPawn->Controller;
	// if (!pawnController)
	// 	return;
	//
	// APlayerController* pawnPlayerController = Cast<APlayerController>(pawnController);
	// if (!pawnPlayerController)
	// 	return;
	//
	// FHitResult hitResult;
	// pawnPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, hitResult);
	//
	// if (!hitResult.bBlockingHit)
	// 	return;


}
