// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/FSEquipItem.h"
#include "FSPlayerController.h"
#include "FSAssetManager.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "GGJLostThings/GGJLostThingsCharacter.h"


void UFSEquipItem::Use(AGGJLostThingsCharacter* Character, FFSItemData ItemDataSelected)
{
	if (Character) {
	}
}

bool UFSEquipItem::Equip(AGGJLostThingsCharacter* Character, FFSItemSlot ItemSlot)
{
	return true;
}

bool UFSEquipItem::UnEquip(AGGJLostThingsCharacter* Character, FFSItemSlot ItemSlot)
{
	return false;
}
