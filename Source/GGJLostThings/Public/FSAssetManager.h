// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "FSAssetManager.generated.h"

class UFSItem;

/**
 * 
 */
UCLASS()
class GGJLOSTTHINGS_API UFSAssetManager : public UAssetManager
{
	GENERATED_BODY()
	

public:
	// Constructor and overrides
	UFSAssetManager() {}
	virtual void StartInitialLoading() override;

	/** Static types for items */
	static const FPrimaryAssetType	LostItemType;
	static const FPrimaryAssetType	ConsumableItemType;
	/** Returns the current AssetManager object */
	static UFSAssetManager& Get();

	/**
	 * Synchronously loads an FSItem subclass, this can hitch but is useful when you cannot wait for an async load
	 * This does not maintain a reference to the item so it will garbage collect if not loaded some other way
	 *
	 * @param PrimaryAssetId The asset identifier to load
	 * @param bDisplayWarning If true, this will log a warning if the item failed to load
	 */
	UFSItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};