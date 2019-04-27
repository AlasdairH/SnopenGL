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

	class Log
	{
	public:
		Log() { }
		~Log();

		std::ostringstream & getBuffer(const LogLevel &_logLevel);

		static LogLevel reportingLevel;
	protected:
		std::ostringstream m_oStream;

	};
}
