
#include "Items/P2DRPGItem.h"

bool UP2DRPGItem::IsConsumable() const
{
	if (MaxCount <= 0)
	{
		return true;
	}
	return false;
}

FString UP2DRPGItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UP2DRPGItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}