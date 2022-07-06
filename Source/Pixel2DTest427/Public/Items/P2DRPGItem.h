#pragma once

#include "Pixel2DTest427/Pixel2DTest427.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "Assets/P2DAssetManager.h"
#include "P2DRPGItem.generated.h"

/** Base class for all items, do not blueprint directly */
UCLASS(Abstract, BlueprintType)
class PIXEL2DTEST427_API UP2DRPGItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Constructor */
	UP2DRPGItem() :
		MaxCount(1)
	{}

	/** Type of this item, set in native parent class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FSlateBrush ItemIcon;
	
	/** Maximum number of instances that can be in inventory at once, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxCount;

	/** Returns if the item is consumable (MaxCount <= 0)*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Max)
	bool IsConsumable() const;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};


