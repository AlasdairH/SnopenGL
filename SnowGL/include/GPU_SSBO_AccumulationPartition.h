#pragma once

// cstdlib


// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class GPU_SSBO_accumulationPartition
	*	@brief The CPU side data for the acumulation buffer
	*
	*	This data corresponds to the shader side "buffer_accumulation" for the acumulation spatial partition
	*	All vec3s are vec4s for padding.
	*/
	struct GPU_SSBO_accumulationPartition
	{
		glm::vec4 dimensions;			// the size of the spatial partition			
		glm::vec4 resolution;			// the number of partitions in the width, height and depth
		glm::vec4 position;				// the position of the spatial partition

		// pre compute
		glm::vec4 positionBL;			// the position of the bottom left corner
		glm::vec4 binSize;				// individual bin size

		int bin[100000];				// the array of bins
	};
}