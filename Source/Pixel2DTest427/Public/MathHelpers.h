#pragma once

namespace FMathEX
{
	FORCEINLINE FVector NormalizedMultiplication(FVector A, FVector B)
	{
		const FVector NormalizedA = A.GetSafeNormal(KINDA_SMALL_NUMBER);
		const int RoundedXFromA = FMath::RoundToInt(NormalizedA.X);
		const float ResultX = B.X * RoundedXFromA * -1.0f;

		return FVector(ResultX, B.Y, B.Z);
	}

	FORCEINLINE float SelectRandomFloat(float A, float B)
	{
		return FMath::RandBool() ? A : B;
	}
};
