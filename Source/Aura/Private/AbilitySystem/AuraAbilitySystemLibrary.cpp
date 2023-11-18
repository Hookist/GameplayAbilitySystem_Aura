// Copyright Stas Company


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	if (!playerController) return nullptr;

	if (auto auraHUD = Cast<AAuraHUD>(playerController->GetHUD()))
	{
		auto playerState = playerController->GetPlayerState<AAuraPlayerState>();
		auto aSC = playerState->GetAbilitySystemComponent();
		UAttributeSet* attributeSet = playerState->GetAttributeSet();
		const FWidgetControllerParams WidgetControllerParams(playerController, playerState, aSC, attributeSet);
		return auraHUD->GetOverlayWidgetController(WidgetControllerParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	if (!playerController) return nullptr;

	if (auto auraHUD = Cast<AAuraHUD>(playerController->GetHUD()))
	{
		auto playerState = playerController->GetPlayerState<AAuraPlayerState>();
		auto aSC = playerState->GetAbilitySystemComponent();
		UAttributeSet* attributeSet = playerState->GetAttributeSet();
		const FWidgetControllerParams WidgetControllerParams(playerController, playerState, aSC, attributeSet);
		return auraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
	}
	return nullptr;
}
