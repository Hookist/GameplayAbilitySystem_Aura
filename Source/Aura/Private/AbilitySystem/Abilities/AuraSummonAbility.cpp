// Copyright Stas Company


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float deltaSpread = SpawnSpread / NumMinions;

	const FVector leftOfSpread = forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	TArray<FVector> spawnLocation;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector direction = leftOfSpread.RotateAngleAxis(deltaSpread * i, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), location,
										 location + direction * MaxSpawnDistance,
										 8.f, FLinearColor::Green, 3.f);

		FVector chosenSpawnLocation = location + direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult hit;
		GetWorld()->LineTraceSingleByChannel(hit, chosenSpawnLocation + FVector(0.f, 0.f, 400.f),
			chosenSpawnLocation + FVector(0.f, 0.f, 400.f), ECC_Visibility);

		if (hit.bBlockingHit)
		{
			chosenSpawnLocation = hit.ImpactPoint;
		}
		spawnLocation.Add(chosenSpawnLocation);
	}

	return spawnLocation;
}
