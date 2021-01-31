// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Ability/FSAbilityTypes.h"
#include "FSTargetType.generated.h"

class AFSCharacterBase;
class AActor;
struct FGameplayEventData;

/**
 * 
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class GGJLOSTTHINGS_API UFSTargetType : public UObject
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UFSTargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(AFSCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class GGJLOSTTHINGS_API UFSTargetType_UseOwner : public UFSTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UFSTargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(AFSCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class GGJLOSTTHINGS_API UFSTargetType_UseEventData : public UFSTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UFSTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(AFSCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};