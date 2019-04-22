#pragma once

// cstdlib


// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class GPU_Timer
	*	@brief Allows for timing of GPU execution through OpenGL Queries
	*
	*	This class can time GPU operations and executions through OpenGL Queries.
	*/
	class GPU_Timer
	{
	public:
		/** @brief Constructor - Creates the query
		*
		*	The constructor creates the query
		*/
		GPU_Timer();
		/** @brief Destructor - Removes the query
		*
		*	The destructor deletes the query
		*/
		~GPU_Timer();

		/** @brief Starts the timer
		*
		*	Begins the timer by beginning the query
		*/
		void start();
		/** @brief Ends the timer
		*
		*	Ends the timer by ending the query
		*/
		void end();

		/** @brief Returns the running state of the timer
		*	@return The running state of the query
		*
		*	Returns whether the timer is running or not
		*/
		inline bool isRunning() { return m_started; }

		/** @brief Returns the timers duration in nanoseconds
		*	@return The timer duration in nanoseconds
		*
		*	Returns the timers duration in nanoseconds. The timer must be stopped for this to work and it is recommended the duration only be gathered at the end of a frame
		*	to avoid syncing bottlenecks.
		*/
		unsigned int getDuration();

	protected:
		bool	m_started = false;			/**< The state of the timer */
		GLuint	m_query;					/**< The ID of the OpenGL Query */
		GLuint	m_duration;					/**< The last recorded duration of the timer in nanoseconds */
	};
}