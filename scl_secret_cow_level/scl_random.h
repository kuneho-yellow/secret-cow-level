#pragma once

class SCLRandom
{
public:
	static void Initialize();
	static int GetRandomInt(int Min, int Max);
	static float GetRandomFloat(float Min, float Max);
};

