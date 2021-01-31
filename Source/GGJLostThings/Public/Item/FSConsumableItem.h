// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FSItem.h"
#include "FSTypes.h"
#include "FSAssetManager.h"
#include "FSConsumableItem.generated.h"

/**
 * 
 */
UCLASS()
class GGJLOSTTHINGS_API UFSConsumableItem : public UFSItem
{
	GENERATED_BODY()
public:
	UFSConsumableItem()
	{
		ItemType = UFSAssetManager::ConsumableItemType;
	}

	virtual void Use(class AGGJLostThingsCharacter* Character, FFSItemData ItemDataSelected) override;
	
};
