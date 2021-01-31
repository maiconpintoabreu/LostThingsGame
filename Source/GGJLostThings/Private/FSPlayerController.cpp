// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPlayerController.h"
#include "FSCharacterBase.h"
#include "FSGameInstance.h"
#include "FSSaveGame.h"
#include "Item/FSItem.h"
#include "FSAssetManager.h"


bool AFSPlayerController::AddInventoryItem(UFSItem* NewItem, int32 Id, int32 ItemCount, int32 ItemLevel, int32 ItemDurability, bool bAutoSlot)
{
	bool bChanged = false;
	if (!NewItem)
	{
		return false;
	}

	if (ItemCount <= 0 || ItemLevel <= 0)
	{
		return false;
	}

	// Find current item data, which may be empty
	FFSItemData OldData;
	GetInventoryItemData(NewItem, OldData);

	// Find modified data
	FFSItemData NewData = OldData;
	NewData.UpdateItemData(FFSItemData(ItemCount, ItemLevel), NewItem->MaxCount, NewItem->MaxLevel);

	if (OldData != NewData)
	{
		// If data changed, need to update storage and call callback
		InventoryData.Add(NewItem, NewData);
		NotifyInventoryItemChanged(true, NewItem);
		bChanged = true;
	}

	if (bAutoSlot)
	{
		// Slot item if required
		bChanged |= FillEmptySlotWithItem(NewItem);
	}

	if (bChanged)
	{
		// If anything changed, write to save game
		SaveInventory();
		return true;
	}
	return false;
}

//bool AFSPlayerController::RemoveInventoryItem(UFSItem* RemovedItem, int32 RemoveCount)
bool AFSPlayerController::RemoveInventoryItem(UFSItem* RemovedItem, FFSItemData RemoveItemData, int32 RemoveCount)
{
	if (!RemovedItem)
	{
		return false;
	}

	// Find current item data, which may be empty
	FFSItemData NewData;
	GetInventoryItemData(RemovedItem, NewData);

	if (!NewData.IsValid())
	{
		// Wasn't found
		return false;
	}

	// If RemoveCount <= 0, delete all
	if (RemoveCount <= 0)
	{
		NewData.ItemCount = 0;
	}
	else
	{
		NewData.ItemCount -= RemoveCount;
	}

	if (NewData.ItemCount > 0)
	{
		// Update data with new count
		InventoryData.Add(RemovedItem, NewData);
	}
	else
	{
		// Remove item entirely, make sure it is unslotted
		InventoryData.Remove(RemovedItem);

		for (TPair<FFSItemSlot, UFSItem*>& Pair : SlottedItems)
		{
			if (Pair.Value == RemovedItem)
			{
				Pair.Value = nullptr;
				NotifySlottedItemChanged(Pair.Key, Pair.Value);
			}
		}
	}

	// If we got this far, there is a change so notify and save
	NotifyInventoryItemChanged(false, RemovedItem);

	SaveInventory();
	return true;
}

void AFSPlayerController::GetInventoryItems(TArray<UFSItem*>& Items, FPrimaryAssetType ItemType)
{
	for (const TPair<UFSItem*, FFSItemData>& Pair : InventoryData)
	{
		if (Pair.Key)
		{
			FPrimaryAssetId AssetId = Pair.Key->GetPrimaryAssetId();

			// Filters based on item type
			if (AssetId.PrimaryAssetType == ItemType || !ItemType.IsValid())
			{
				Items.Add(Pair.Key);
			}
		}
	}
}

bool AFSPlayerController::SetSlottedItem(FFSItemSlot ItemSlot, UFSItem* Item, FFSItemData ItemData)
{
	// Iterate entire inventory because we need to remove from old slot
	bool bFound = false;
	for (TPair<FFSItemSlot, UFSItem*>& Pair : SlottedItems)
	{
		if (Pair.Key == ItemSlot)
		{
			// Add to new slot
			bFound = true;
			Pair.Value = Item;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
		else if (Item != nullptr && Pair.Value == Item)
		{
			// If this item was found in another slot, remove it
			Pair.Value = nullptr;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
	}

	if (bFound)
	{
		SaveInventory();
		return true;
	}

	return false;
}

bool AFSPlayerController::GetInventoryItemData(UFSItem* Item, FFSItemData& ItemData) const
{
	const FFSItemData* FoundItem = InventoryData.Find(Item);
	if (FoundItem)
	{
		ItemData = *FoundItem;
		return true;
	}
	ItemData = FFSItemData(0,0);
	return false;
}

UFSItem* AFSPlayerController::GetSlottedItem(FFSItemSlot ItemSlot) const
{
	UFSItem* const* FoundItem = SlottedItems.Find(ItemSlot);

	if (FoundItem)
	{
		return *FoundItem;
	}
	return nullptr;
}

void AFSPlayerController::GetSlottedItems(TArray<UFSItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes)
{
	for (TPair<FFSItemSlot, UFSItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == ItemType || !ItemType.IsValid())
		{
			Items.Add(Pair.Value);
		}
	}
}

void AFSPlayerController::FillEmptySlots()
{
	bool bShouldSave = false;
	for (const TPair<UFSItem*, FFSItemData>& Pair : InventoryData)
	{
		bShouldSave |= FillEmptySlotWithItem(Pair.Key);
	}

	if (bShouldSave)
	{
		SaveInventory();
	}
}

bool AFSPlayerController::SaveInventory()
{
	UWorld* World = GetWorld();
	UFSGameInstance* GameInstance = World ? World->GetGameInstance<UFSGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	UFSSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame)
	{
		// Reset cached data in save game before writing to it
		CurrentSaveGame->InventoryData.Reset();
		CurrentSaveGame->SlottedItems.Reset();

		for (const TPair<UFSItem*, FFSItemData>& ItemPair : InventoryData)
		{
			FPrimaryAssetId AssetId;

			if (ItemPair.Key)
			{
				AssetId = ItemPair.Key->GetPrimaryAssetId();
				CurrentSaveGame->InventoryData.Add(AssetId, ItemPair.Value);
			}
		}

		for (const TPair<FFSItemSlot, UFSItem*>& SlotPair : SlottedItems)
		{
			FPrimaryAssetId AssetId;

			if (SlotPair.Value)
			{
				AssetId = SlotPair.Value->GetPrimaryAssetId();
			}
			CurrentSaveGame->SlottedItems.Add(SlotPair.Key, AssetId);
		}

		// Now that cache is updated, write to disk
		GameInstance->WriteSaveGame();
		return true;
	}
	return false;
}

bool AFSPlayerController::LoadInventory()
{

	InventoryData.Reset();
	SlottedItems.Reset();

	// Fill in slots from game instance
	UWorld* World = GetWorld();
	UFSGameInstance* GameInstance = World ? World->GetGameInstance<UFSGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	// Bind to loaded callback if not already bound
	if (!GameInstance->OnSaveGameLoadedNative.IsBoundToObject(this))
	{
		GameInstance->OnSaveGameLoadedNative.AddUObject(this, &AFSPlayerController::HandleSaveGameLoaded);
	}

	for (const TPair<FPrimaryAssetType, int32>& Pair : GameInstance->ItemSlotsPerType)
	{
		for (int32 SlotNumber = 0; SlotNumber < Pair.Value; SlotNumber++)
		{
			SlottedItems.Add(FFSItemSlot(Pair.Key, SlotNumber), nullptr);
		}
	}

	UFSSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	UFSAssetManager& AssetManager = UFSAssetManager::Get();
	if (CurrentSaveGame)
	{
		// Copy from save game into controller data
		bool bFoundAnySlots = false;
		for (const TPair<FPrimaryAssetId, FFSItemData>& ItemPair : CurrentSaveGame->InventoryData)
		{
			UFSItem* LoadedItem = AssetManager.ForceLoadItem(ItemPair.Key);

			if (LoadedItem != nullptr)
			{
				InventoryData.Add(LoadedItem, ItemPair.Value);
			}
		}

		for (const TPair<FFSItemSlot, FPrimaryAssetId>& SlotPair : CurrentSaveGame->SlottedItems)
		{
			if (SlotPair.Value.IsValid())
			{
				UFSItem* LoadedItem = AssetManager.ForceLoadItem(SlotPair.Value);
				if (GameInstance->IsValidItemSlot(SlotPair.Key) && LoadedItem)
				{
					SlottedItems.Add(SlotPair.Key, LoadedItem);
					bFoundAnySlots = true;
				}
			}
		}

		if (!bFoundAnySlots)
		{
			// Auto slot items as no slots were saved
			FillEmptySlots();
		}

		NotifyInventoryLoaded();

		return true;
	}

	// Load failed but we reset inventory, so need to notify UI
	NotifyInventoryLoaded();

	return false;
}

bool AFSPlayerController::FillEmptySlotWithItem(UFSItem* NewItem)
{
	// Look for an empty item slot to fill with this item
	FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;
	FFSItemSlot EmptySlot;
	for (TPair<FFSItemSlot, UFSItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == NewItemType)
		{
			if (Pair.Value == NewItem)
			{
				// Item is already slotted
				return false;
			}
			else if (Pair.Value == nullptr && (!EmptySlot.IsValid() || EmptySlot.SlotNumber > Pair.Key.SlotNumber))
			{
				// We found an empty slot worth filling
				EmptySlot = Pair.Key;
			}
		}
	}

	if (EmptySlot.IsValid())
	{
		SlottedItems[EmptySlot] = NewItem;
		NotifySlottedItemChanged(EmptySlot, NewItem);
		return true;
	}

	return false;
}

void AFSPlayerController::NotifyInventoryItemChanged(bool bAdded, UFSItem* Item)
{
	// Notify native before blueprint
	OnInventoryItemChangedNative.Broadcast(bAdded, Item);
	OnInventoryItemChanged.Broadcast(bAdded, Item);

	// Call BP update event
	InventoryItemChanged(bAdded, Item);
}

void AFSPlayerController::NotifySlottedItemChanged(FFSItemSlot ItemSlot, UFSItem* Item)
{
	// Notify native before blueprint
	OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
	OnSlottedItemChanged.Broadcast(ItemSlot, Item);

	// Call BP update event
	SlottedItemChanged(ItemSlot, Item);
}

void AFSPlayerController::NotifyInventoryLoaded()
{
	// Notify native before blueprint
	OnInventoryLoadedNative.Broadcast();
	OnInventoryLoaded.Broadcast();
}

void AFSPlayerController::HandleSaveGameLoaded(UFSSaveGame* NewSaveGame)
{
	LoadInventory();
}

void AFSPlayerController::BeginPlay()
{
	// Load inventory off save game before starting play
	LoadInventory();

	Super::BeginPlay();
}

