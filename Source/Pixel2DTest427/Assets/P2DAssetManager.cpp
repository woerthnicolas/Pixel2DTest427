#include "Assets/P2DAssetManager.h"
#include "Items/P2DRPGItem.h"

const FPrimaryAssetType	UP2DAssetManager::PotionItemType = TEXT("Potion");


UP2DAssetManager& UP2DAssetManager::Get()
{
	UP2DAssetManager* This = Cast<UP2DAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be P2DAssetsManager!"));
		return *NewObject<UP2DAssetManager>(); // never calls this
	}
}

void UP2DAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}


UP2DRPGItem* UP2DAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{	
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UP2DRPGItem* LoadedItem = Cast<UP2DRPGItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}


