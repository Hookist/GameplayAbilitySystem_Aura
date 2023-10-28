// Copyright Stas Company


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	widget->AddToViewport();
	
}
