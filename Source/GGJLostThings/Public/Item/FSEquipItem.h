// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FSItem.h"
#include "FSTypes.h"
#include "FSEquipItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, DefaultToInstanced, EditInlineNew)
class GGJLOSTTHINGS_API UFSEquipItem : public UFSItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipToSpawn")
	TSubclassOf<AActor> EquipClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Equip")
	TMap<FFSItemSlot, class AActor*> EquipActorArray;
	
	virtual void Use(class AGGJLostThingsCharacter* Character, FFSItemData ItemDataSelected) override;

	UFUNCTION(BlueprintCallable)
	bool Equip(AGGJLostThingsCharacter* Character, FFSItemSlot ItemSlot);
	
	UFUNCTION(BlueprintCallable)
	bool UnEquip(AGGJLostThingsCharacter* Character, FFSItemSlot ItemSlot);

};
