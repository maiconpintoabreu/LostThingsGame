// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/FSConsumableItem.h"
#include "FSPlayerController.h"
#include "GGJLostThings/GGJLostThingsCharacter.h"


void UFSConsumableItem::Use(AGGJLostThingsCharacter* Character, FFSItemData ItemDataSelected)
{
	if (Character) {
		AFSPlayerController* PlayerController = Cast<AFSPlayerController>(Character->GetController());
		if (PlayerController) {
			if (Character->ActivateAbilitiesWithItem(this, true)) {
				PlayerController->RemoveInventoryItem(this, ItemDataSelected, 1);
			}
		}
	}
}