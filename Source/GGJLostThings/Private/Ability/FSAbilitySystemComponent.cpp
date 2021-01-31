// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/FSAbilitySystemComponent.h"
#include "FSCharacterBase.h"
#include "Ability/FSGameplayAbility.h"
#include "AbilitySystemGlobals.h"

UFSAbilitySystemComponent::UFSAbilitySystemComponent() {}

void UFSAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UFSGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UFSGameplayAbility>(ActiveAbility));
		}
	}
}

int32 UFSAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	AFSCharacterBase* OwningCharacter = Cast<AFSCharacterBase>(GetOwnerActor());

	if (OwningCharacter)
	{
		return OwningCharacter->GetCharacterLevel();
	}
	return 1;
}

UFSAbilitySystemComponent* UFSAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<UFSAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}