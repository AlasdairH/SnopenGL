#pragma once

// cstdlib

// external libs


// program
#include "PCH.h"

namespace SnowGL
{
	class Utils
	{
	public:
		static float randFloat(const float _min, const float _max)
		{
			return _min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_max - _min)));
		}
	};
}