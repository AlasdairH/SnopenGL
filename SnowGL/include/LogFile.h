#pragma once

// cstdlib


// external libs

// program
#include "PCH.h"

// TODO: Doxygen

namespace SnowGL
{
	class LogFile
	{
	public:
		LogFile(const std::string &_filepath);
		~LogFile();

		void write(const std::string &_line);

	protected:
		std::ofstream m_file;
	};
}