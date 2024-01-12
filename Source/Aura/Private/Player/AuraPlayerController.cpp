// Copyright Stas Company


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/Interactable.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning)
		return;
	
	if (APawn* controlledPawn = GetPawn())
	{
		const FVector locationOnSpline =
			Spline->FindLocationClosestToWorldLocation(controlledPawn->GetActorLocation(),
			ESplineCoordinateSpace::World);
		const FVector direction =
			Spline->FindDirectionClosestToWorldLocation(locationOnSpline, ESplineCoordinateSpace::World);
		controlledPawn->AddMovementInput(direction);

		const float distanceToDestination = (locationOnSpline - CachedDestination).Length();
		if (distanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (subsystem)
	{
		subsystem->AddMappingContext(AuraContext, 0);	
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI inputModeData;
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(inputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto auraInputComponent
		= CastChecked<UAuraInputComponent>(InputComponent);

	auraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	auraInputComponent->BindAbilityAction(
		InputConfig,
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D inputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator rotation = GetControlRotation();
	const FRotator yawRotation(0.f, rotation.Yaw, 0.f);

	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	if (auto controlledPawn = GetPawn<APawn>())
	{
		controlledPawn->AddMovementInput(forwardDirection, inputAxisVector.Y);
		controlledPawn->AddMovementInput(rightDirection, inputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IInteractable>(CursorHit.GetActor());

	/**
	 * Line trace from cursor. There are several scenariosL
	 *  A. LastActor is null && CurrentActor is null
	 *		- Do nothing.
	 *	B. LastActor is null && CurrentActor is valid
	 *		- Highlight CurrentActor
	 *	C. LastActor is valid && CurrentActor is null
	 *		- Unhighlight LastActor
	 *	D. Both actors are valid, but LastActor != CurentActor
	 *	    - Unhighlight LastActor, and Highlight CurrentActor
	 *	E. Both actors are valid, but LastActor == CurentActor
	 *		- Do nothing
	 */

	// if (LastActor == nullptr)
	// {
	// 	if (CurrentActor != nullptr)
	// 	{
	// 		// Case B
	// 		CurrentActor->HighlightActor();
	// 	}
	// 	else
	// 	{
	// 		// Case A - both are null, do nothing
	// 	}
	// }
	// else // LastActor is valid
	// {
	// 	if (CurrentActor == nullptr)
	// 	{
	// 		// Case C
	// 		LastActor->UnHighlightActor();
	// 	}
	// 	else // both actors are valid
	// 	{
	// 		if (LastActor != CurrentActor)
	// 		{
	// 			// Case D
	// 			LastActor->UnHighlightActor();
	// 			CurrentActor->HighlightActor();
	// 		}
	// 		else
	// 		{
	// 			// Case E - do nothing
	// 		}
	// 	}
	// }

	if (LastActor != CurrentActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (CurrentActor) CurrentActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = CurrentActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}
	
	if (bTargeting)
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	}
	else
	{
		const APawn* controllerPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && controllerPawn)
		{
			if (UNavigationPath* navPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
				controllerPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();

				for (const FVector& pointLoc : navPath->PathPoints)
				{
					Spline->AddSplinePoint(pointLoc, ESplineCoordinateSpace::World);
				}
				CachedDestination = navPath->PathPoints[navPath->PathPoints.Num() - 1];
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		
		return;
	}

	if (bTargeting)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* controllerPawn = GetPawn())
		{
			const FVector worldDirection = (CachedDestination - controllerPawn->GetActorLocation()).GetSafeNormal();
			controllerPawn->AddMovementInput(worldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		auto asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>());
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(asc);
	}
	return AuraAbilitySystemComponent;
}

