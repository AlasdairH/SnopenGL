#include "PCH.h"
#include "Log.h"

namespace SnowGL
{
#ifdef COMPILE_INCLUDE_LOGGING
	LogLevel Log::reportingLevel = LOG_DEBUG;
#else
	LogLevel Log::reportingLevel = LOG_WARNING;
#endif

	Log::~Log()
	{
		std::cout << m_oStream.str() << "\n";
	}

	std::ostringstream & Log::getBuffer(const LogLevel &_logLevel)
	{
		if (_logLevel == LOG_DEBUG)
		{
			m_oStream << "DEBUG";
		}
		else if (_logLevel == LOG_WARNING)
		{
			m_oStream << "WARNING";
		}
		else if (_logLevel == LOG_ERROR)
		{
			m_oStream << "ERROR";
		}
		else
		{
			m_oStream << "UNKNOWN";
		}
		return m_oStream;
	}
}