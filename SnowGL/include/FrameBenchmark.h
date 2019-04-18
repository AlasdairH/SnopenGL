#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

// TODO: Doxygen

namespace SnowGL
{
	struct FrameBenchmark
	{
		unsigned int	frame										= 0;
		GPU_Timer		shadowMapping;
		GPU_Timer		collisionDetectionTransformFeedback;
		GPU_Timer		visuals;
		GPU_Timer		particleSimulation;

		// text
		std::stringstream getDataSS();

		// csv methods
		std::stringstream getCSVHeaders();
		std::stringstream getDataCSV();
	};
}