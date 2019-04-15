#include "scl_random.h"

#include <stdlib.h>
#include <ctime>

void SCLRandom::Initialize()
{
	srand((unsigned int)time(NULL));
}

// NOTE: Need better algo for real game!!!

int SCLRandom::GetRandomInt(int Min, int Max)
{
	int Range = Max - Min + 1;
	return rand() % Range + Min;
}

float SCLRandom::GetRandomFloat(float Min, float Max)
{
	float Range = Max - Min;
	return (rand() * Range / (float)RAND_MAX) + Min;
}