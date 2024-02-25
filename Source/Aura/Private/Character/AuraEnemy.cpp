// Copyright Stas Company


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetMesh());
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	
	if (auto auraUserWidget = Cast<UAuraUserWidget>(HealthWidgetComponent->GetUserWidgetObject()))
	{
		auraUserWidget->SetWidgetController(this);
	}
	
	if (const UAuraAttributeSet* auraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FAuraGameplayTags::Get().Effects_HitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::HitReactTagChanged);
		
		OnHealthChanged.Broadcast(auraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(auraAS->GetMaxHealth());
	}
}

int32 AAuraEnemy::GetCreatureLevel()
{
	return Level;
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

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}
