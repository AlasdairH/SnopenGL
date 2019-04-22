#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class FrameBenchmark
	*	@brief A strust containing all the data for measuring the performance of a single frame of rendering
	*
	*	This struct contains timers and other data storage for the measurement of a frame of rendering. 
	*/
	struct FrameBenchmark
	{
		unsigned int	frame									= 0;	/**< The current frame number, starts from 0 and is increased every frame */
		GPU_Timer		shadowMapping;									/**< The time taken to execute the shadow mapping */
		GPU_Timer		collisionDetectionTransformFeedback;			/**< The time taken to execute the transform feedback of the world space geometry */
		GPU_Timer		visuals;										/**< The time taken to execute the visual rendering to the primary framebuffer */
		GPU_Timer		particleSimulation;								/**< The time taken to execute the particle simulation, including collision detection */

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