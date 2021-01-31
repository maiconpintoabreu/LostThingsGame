// Fill out your copyright notice in the Description page of Project Settings.


#include "FSSaveGame.h"
#include "FSGameInstance.h"

void UFSSaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading() && SavedDataVersion != EFSSaveGameVersion::LatestVersion)
	{
		if (SavedDataVersion < EFSSaveGameVersion::AddedItemData)
		{
			// Convert from list to item data map
			for (const FPrimaryAssetId& ItemId : InventoryItems)
			{
				InventoryData.Add(ItemId, FFSItemData());
			}

			InventoryItems.Empty();
		}
		
		SavedDataVersion = EFSSaveGameVersion::LatestVersion;
	}
}
