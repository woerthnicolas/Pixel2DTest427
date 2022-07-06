#pragma once

#include "DelayAction.h"
#include "PaperSprite.h"

namespace Utilities
{
	FORCEINLINE FSlateBrush MakeBrushFromSprite(UPaperSprite* Sprite, int32 Width, int32 Height)
	{
		if (Sprite)
		{
			const FSlateAtlasData SpriteAtlasData = Sprite->GetSlateAtlasData();
			const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();

			FSlateBrush Brush;
			Brush.SetResourceObject(Sprite);
			Width = (Width > 0) ? Width : SpriteSize.X;
			Height = (Height > 0) ? Height : SpriteSize.Y;
			Brush.ImageSize = FVector2D(Width, Height);
			return Brush;
		}

		return FSlateNoResource();
	}
}
