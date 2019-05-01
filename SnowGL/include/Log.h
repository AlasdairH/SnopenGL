#pragma once

// PCH
#include "PCH.h"

// Program

namespace SnowGL
{
#define LOG(level) \
	if(level < Log::reportingLevel); \
	else Log().getBuffer(level) << "(" << __func__ << ":" << __LINE__ << "): " \

	enum LogLevel { LOG_DEBUG = 0, LOG_WARNING = 1, LOG_ERROR = 2 };

	/*! @class Log
	*	@brief Handles logging at varying levels
	*
	*	The log class contains methods for logging to the console for ease of debugging and error messaging.
	*/
	class Log
	{
	public:
		/** @brief Log Constructor
		*
		*	Does nothing
		*/
		Log() { }
		/** @brief Log Deconstructor
		*
		*	Outputs the buffer stream to the console
		*/
		~Log();

		/** @brief Returns a reference to the buffer for the log
		*	@param _logLevel The level at which to set this log line to
		*	@return The buffer stream which can be written too through the << operator
		*
		*	Gets a reference to the buffer which can be written to at the specified log level
		*/
		std::ostringstream & getBuffer(const LogLevel &_logLevel);

		static LogLevel reportingLevel;		/**< The log reporting level, any below will be ignored */
	protected:
		std::ostringstream m_oStream;		/**< The string buffer containing the log */

	};
}
