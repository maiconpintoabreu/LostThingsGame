// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/FSItem.h"
#include "GGJLostThings/GGJLostThingsCharacter.h"
#include "FSAssetManager.h"

UFSItem::UFSItem() {
	Weight = 1.f;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}

void UFSItem::Use(AGGJLostThingsCharacter* Character, FFSItemData ItemDataSelected)
{
	OnUse(Character,ItemDataSelected,this);
}

void UFSItem::Pickup(AGGJLostThingsCharacter* Character)
{

}

FString UFSItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UFSItem::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(ItemType, GetFName());
}