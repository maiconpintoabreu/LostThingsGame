// Fill out your copyright notice in the Description page of Project Settings.


#include "FSAssetManager.h"
#include "Item/FSItem.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType	UFSAssetManager::LostItemType = TEXT("LostItem");
const FPrimaryAssetType	UFSAssetManager::ConsumableItemType = TEXT("ConsumableItem");

UFSAssetManager& UFSAssetManager::Get()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting FSAssetManager (Get)!"));
	UFSAssetManager* This = Cast<UFSAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be FSAssetManager!"));
		return *NewObject<UFSAssetManager>(); // never calls this
	}
}

void UFSAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UE_LOG(LogTemp, Warning, TEXT("Init!"));

	UAbilitySystemGlobals::Get().InitGlobalData();
}


UFSItem* UFSAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{	
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UFSItem* LoadedItem = Cast<UFSItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadedItem"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}

