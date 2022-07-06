#pragma once

#include "Items/P2DRPGItem.h"
#include "P2DPotionItem.generated.h"

/** Native base class for potions, should be blueprinted */
UCLASS()
class PIXEL2DTEST427_API UP2DPotionItem : public UP2DRPGItem
{
	GENERATED_BODY()

public:
	/** Constructor */
	UP2DPotionItem()
	{
		ItemType = UP2DAssetManager::PotionItemType;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value = 20.0f;
};