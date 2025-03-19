#pragma once
namespace RandomFuncs
{
	int randomRangeInt(int min, int max)
	{
		return min + (rand() % (1 + max - min));
	}

	int randomRangeFloat(float min, float max)
	{
		return  min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
	}
}

