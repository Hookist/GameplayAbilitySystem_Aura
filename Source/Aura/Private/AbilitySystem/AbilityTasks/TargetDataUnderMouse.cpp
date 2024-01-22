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
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		//TODO: We are on the server, so listen for target data.
	}
	
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

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow scopedPrediction(AbilitySystemComponent.Get());
	
	FVector hitLocation;
	APlayerController* pawnPlayerController = Ability->GetActorInfo().PlayerController.Get();
	FHitResult cursorHit;
	pawnPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
	hitLocation = cursorHit.Location;
	
	FGameplayAbilityTargetDataHandle dataHandle;	
	FGameplayAbilityTargetData_SingleTargetHit* data = new FGameplayAbilityTargetData_SingleTargetHit();
	data->HitResult = cursorHit;
	dataHandle.Add(data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		data,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(dataHandle);
	}
}
