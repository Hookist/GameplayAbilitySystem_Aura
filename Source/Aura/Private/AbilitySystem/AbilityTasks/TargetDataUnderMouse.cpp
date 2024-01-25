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
		const FGameplayAbilitySpecHandle specHandle = GetAbilitySpecHandle();
		const FPredictionKey activationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(specHandle, activationPredictionKey)
			.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(specHandle, activationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
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
	
	APlayerController* pawnPlayerController = Ability->GetActorInfo().PlayerController.Get();
	FHitResult cursorHit;
	pawnPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
	
	FGameplayAbilityTargetDataHandle dataHandle;	
	auto data = new FGameplayAbilityTargetData_SingleTargetHit();
	data->HitResult = cursorHit;
	dataHandle.Add(data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		dataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(dataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
