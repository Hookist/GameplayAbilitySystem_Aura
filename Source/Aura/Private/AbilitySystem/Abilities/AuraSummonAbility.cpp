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

		const FVector chosenSpawnLocation = location + direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		spawnLocation.Add(chosenSpawnLocation);
		DrawDebugSphere(GetWorld(), chosenSpawnLocation,
			18.f, 12, FColor::Cyan, false, 3.f);

		DrawDebugSphere(GetWorld(), location + direction * MinSpawnDistance,
			5.f, 12, FColor::Red, false, 3.f);
		DrawDebugSphere(GetWorld(), location + direction * MaxSpawnDistance,
			5.f, 12, FColor::Red, false, 3.f);
	}

	return spawnLocation;
}
