#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "P2DAssetManager.generated.h"

class UP2DRPGItem;

/**
 * 
 */
UCLASS()
class PIXEL2DTEST427_API UP2DAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	// Constructor and overrides
	UP2DAssetManager() {}
	virtual void StartInitialLoading() override;

	/** Static types for items */
	static const FPrimaryAssetType	PotionItemType;
	// static const FPrimaryAssetType	SkillItemType;
	// static const FPrimaryAssetType	TokenItemType;
	// static const FPrimaryAssetType	WeaponItemType;

	/** Returns the current AssetManager object */
	static UP2DAssetManager& Get();

	/**
	 * Synchronously loads an RPGItem subclass, this can hitch but is useful when you cannot wait for an async load
	 * This does not maintain a reference to the item so it will garbage collect if not loaded some other way
	 *
	 * @param PrimaryAssetId The asset identifier to load
	 * @param bDisplayWarning If true, this will log a warning if the item failed to load
	 */
	UP2DRPGItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
