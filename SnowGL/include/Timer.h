#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

/*! @class Timer
*	@brief A precise timer for benchmarking or measurement
*
*	A timer which starts on initialisation and can be queried at any time
*/
class Timer
{
public:
	/** @brief Timer Ctor
	*
	*	Starts the timer
	*/
	Timer() { start = std::chrono::high_resolution_clock::now(); }
	/** @brief Timer Dtor
	*
	*	Stops the timer
	*/
	~Timer() { }

	/** @brief Gets the current duration of the timer
	*
	*	Gets the current duration of the timer
	*/
	std::chrono::duration<float> getDuration() { return std::chrono::high_resolution_clock::now() - start; }

protected:
	std::chrono::time_point<std::chrono::high_resolution_clock>		start;		/**< The transform Feedback shader */
	std::chrono::duration<float>									duration;	/**< The last queried duration */
};