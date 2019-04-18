#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

// TODO: Doxygen

namespace SnowGL
{
	enum LogType { LOG_TEXT, LOG_CSV };

	class LogFile
	{
	public:
		LogFile(const std::string &_filepath, const LogType &_type);
		~LogFile();

		void write(const std::string &_line);

	protected:
		std::ofstream	m_file;
		LogType			m_type = LOG_TEXT;
	};
}