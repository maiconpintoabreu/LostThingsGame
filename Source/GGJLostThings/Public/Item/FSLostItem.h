// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FSEquipItem.h"
#include "FSAssetManager.h"
#include "FSLostItem.generated.h"

/**
 * 
 */
UCLASS()
class GGJLOSTTHINGS_API UFSLostItem : public UFSEquipItem
{
	GENERATED_BODY()
public:
	UFSLostItem()
	{
		ItemType = UFSAssetManager::LostItemType;
	}
	
};
