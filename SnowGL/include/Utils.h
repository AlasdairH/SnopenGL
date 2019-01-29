#pragma once

// cstdlib

// external libs


// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class Utils
	*	@brief Provides basic utility functions
	*
	*	Provides basic and commonly used functions
	*/
	class Utils
	{
	public:
		/** @brief Random float generator
		*	@param _min The minimum bound
		*	@param _max The maximum bound
		*	@return A random float between the min and the max
		*
		*	Generates a random float between the minimum and maximum bounds
		*/
		static float randFloat(const float _min, const float _max)
		{
			return _min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_max - _min)));
		}
	};
}