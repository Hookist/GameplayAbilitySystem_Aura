// Copyright Stas Company


#include "Character/AuraCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AAuraCharacter::AAuraCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArmComponent");
	CameraSpringArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetupAttachment(CameraSpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
