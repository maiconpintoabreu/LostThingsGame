// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FSTypes.h"
#include "FSItem.generated.h"

class UFSGameplayAbility;
class AGGJLostThingsCharacter;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, DefaultToInstanced, EditInlineNew)
class GGJLOSTTHINGS_API UFSItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFSItem();

	/** Type of this item, set in native parent class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	/** Ability level this item grants. <= 0 means the character level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	int32 AbilityLevel;

	/** Ability level this item grants. <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	int32 MaxCount;

	/** Maximum level this item can be, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxLevel;

	/** Maximum level this item can be, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxDurability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText UseActionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	bool IsStackable;

	/** Ability to grant if this item is slotted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<UFSGameplayAbility> GrantedAbility;

	/** Ability to grant if this item is slotted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<UFSGameplayAbility> DualGrantedAbility;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	class UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float Weight;
	
	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	UFUNCTION(BlueprintCallable)
	virtual void Use(AGGJLostThingsCharacter* Character, FFSItemData ItemDataSelected);

	UFUNCTION()
	virtual void Pickup(AGGJLostThingsCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(AGGJLostThingsCharacter* Character, FFSItemData ItemDataSelected, class UFSItem* Item);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickup(AGGJLostThingsCharacter* Character, FFSItemData ItemDataSelected, class UFSItem* Item);

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
