#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class SceneDump
	*	@brief A strust containing all the data for measuring the performance of the scene
	*
	*	This struct contains timers and other data storage for the measurement of the scene
	*/
	struct SceneDump
	{
		int simulatedFrames = 0;			/**< The number of simulated frames */
		int triangleCount = 0;				/**< The scenes triangle count */
		int particleCount = 0;				/**< How many particles were simulated */
		float avgParticleSimTime = 0;		/**< The average time taken to simulate the particle step */
		float avgFPS = 0;					/**< The average FPS during the simulation */

		/** @brief Gets the data in string form
		*	@return A string stream containing the data in a standard string form
		*
		*	Returns a string stream containing all the data for the frame in a standard form.
		*/
		std::stringstream getDataSS();

		/** @brief Gets the headers for the data as CSV
		*	@return A string stream containing the headers for a CSV file
		*
		*	This method returns the headers for a CSV file.
		*/
		std::stringstream getCSVHeaders();
		/** @brief Gets the data in CSV form
		*	@return A string stream containing the data in a CSV format
		*
		*	Returns the data in CSV form for easier loading by other programs.
		*/
		std::stringstream getDataCSV();
	};
}